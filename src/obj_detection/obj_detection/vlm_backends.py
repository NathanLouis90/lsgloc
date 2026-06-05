import abc

# for gemini
from google import genai
from google.genai import types

import json

# for image encoding
import io
import base64
import requests

def _build_prompt(labels: list[str]) -> str:
    if not labels:
        raise ValueError(
            "No landmark labels provided to VLM backend. "
            "Pass --ros-args -p landmark_json_path:=<path/to/landmarks.json>"
        )
    label_list = ", ".join(labels)
    return (
        f"Look for any of these specific objects: {label_list}. "
        "Return ONLY a JSON array for objects you can clearly see, where each element has: "
        "'label' (must be exactly one of the listed names), "
        "'box' ([y1, x1, y2, x2] normalised 0-1000), "
        "'confidence' (float 0.0 to 1.0). "
        "If none of the listed objects are visible, return []. "
        "No markdown, no explanation, just the JSON array."
    )

class VLMBackend(abc.ABC):
    @abc.abstractmethod
    def detect(self, pil_img, img_shape) -> list[dict]:
        """Returns list of {"label": str, "u": int, "v": int, "confidence": float}"""
        pass

    @staticmethod
    def parse_boxes_to_pixel(raw, h, w) -> list[dict]:
        # Gemini sometimes wraps the array in {"objects": [...]}
        if isinstance(raw, dict):
            raw = next(iter(raw.values()))
        # VLM sometimes double-encodes: json.loads returns a string instead of a list
        if isinstance(raw, str):
            try:
                raw = json.loads(raw)
            except Exception:
                return []

        detections = []
        for item in raw:
            if not isinstance(item, dict) or "box" not in item:
                continue
            box = item["box"]
            if isinstance(box, dict):
                box = [box.get("y1", 0), box.get("x1", 0), box.get("y2", 0), box.get("x2", 0)]
            elif isinstance(box, str):
                box = box.split(",")
            elif isinstance(box, list) and len(box) == 1 and isinstance(box[0], str):
                box = box[0].split(",")
            if len(box) != 4:
                print(f"[VLMBackend] Skipping malformed box: {box}")
                continue
            y1, x1, y2, x2 = [float(v) for v in box]
            # VLM sometimes returns 0-1 normalised instead of 0-1000 as prompted
            if max(y1, x1, y2, x2) <= 1.0:
                y1, x1, y2, x2 = y1 * 1000, x1 * 1000, y2 * 1000, x2 * 1000
            u = int(((x1 + x2) / 2) / 1000.0 * w)
            v = int(((y1 + y2) / 2) / 1000.0 * h)
            # box corners in pixel coordinates for debug visualization
            u1 = int(x1 / 1000.0 * w)
            v1 = int(y1 / 1000.0 * h)
            u2 = int(x2 / 1000.0 * w)
            v2 = int(y2 / 1000.0 * h)
            confidence = float(item.get("confidence", 1.0))
            detections.append({
                "label": item["label"], "u": u, "v": v,
                "confidence": confidence,
                "box_px": (u1, v1, u2, v2)
            })

        return detections

class GeminiBackend(VLMBackend):
    def __init__(self, api_key: str, labels: list[str], model: str = "gemini-2.5-flash"):
        self.client = genai.Client(api_key=api_key)
        self.model = model
        self.prompt = _build_prompt(labels)

    def detect(self, pil_img, img_shape) -> list[dict]:
        """
        Ask Gemini to detect objects from the landmark label list in the image.

        Returns a list of dicts:
            [{"label": "chair", "u": 320, "v": 240}, ...]
        """
        h, w = img_shape[:2]

        try:
            response = self.client.models.generate_content(
                model="gemini-2.5-flash",
                contents=[pil_img, self.prompt],
                config=types.GenerateContentConfig(
                    response_mime_type="application/json",
                    temperature=0.0
                )
            )
            raw = json.loads(response.text)

        except Exception as e:
            print(f"VLM call failed as {e}")
            return []

        return self.parse_boxes_to_pixel(raw, h, w)

class OllamaBackend(VLMBackend):
    def __init__(self, labels: list[str], model: str = "llava:13b", url: str = "http://localhost:11434"):
        self.model = model
        self.url = url
        self.prompt = _build_prompt(labels)

    def detect(self, pil_img, img_shape) -> list[dict]:
        # encode the image
        buffer = io.BytesIO()
        pil_img.save(buffer, format='JPEG')
        img_b64 = base64.b64encode(buffer.getvalue()).decode()

        h, w = img_shape[:2]

        # build the payload
        payload = {
            "model" : self.model,
            "prompt" : self.prompt,
            "images" : [img_b64],
            "stream" : False, # for response.json to work
            "options": {"temperature": 0, "seed": 42}
        }

        # send and parse
        try:
            resp = requests.post(f"{self.url}/api/generate", json=payload, timeout=60)
            text = resp.json()['response'].strip()
            if text.startswith("```"):
                text = text.split("```")[1]
                if text.startswith("json"):
                    text = text[4:]
            raw = json.loads(text)
        except Exception as e:
            print(f"OllamaBackend detection failed: {e}")
            return []

        return self.parse_boxes_to_pixel(raw, h, w) # detections