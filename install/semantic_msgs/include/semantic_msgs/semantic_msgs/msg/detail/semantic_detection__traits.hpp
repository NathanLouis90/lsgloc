// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from semantic_msgs:msg/SemanticDetection.idl
// generated code does not contain a copyright notice

#ifndef SEMANTIC_MSGS__MSG__DETAIL__SEMANTIC_DETECTION__TRAITS_HPP_
#define SEMANTIC_MSGS__MSG__DETAIL__SEMANTIC_DETECTION__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "semantic_msgs/msg/detail/semantic_detection__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace semantic_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const SemanticDetection & msg,
  std::ostream & out)
{
  out << "{";
  // member: label
  {
    out << "label: ";
    rosidl_generator_traits::value_to_yaml(msg.label, out);
    out << ", ";
  }

  // member: bearing
  {
    out << "bearing: ";
    rosidl_generator_traits::value_to_yaml(msg.bearing, out);
    out << ", ";
  }

  // member: range
  {
    out << "range: ";
    rosidl_generator_traits::value_to_yaml(msg.range, out);
    out << ", ";
  }

  // member: confidence
  {
    out << "confidence: ";
    rosidl_generator_traits::value_to_yaml(msg.confidence, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const SemanticDetection & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: label
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "label: ";
    rosidl_generator_traits::value_to_yaml(msg.label, out);
    out << "\n";
  }

  // member: bearing
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "bearing: ";
    rosidl_generator_traits::value_to_yaml(msg.bearing, out);
    out << "\n";
  }

  // member: range
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "range: ";
    rosidl_generator_traits::value_to_yaml(msg.range, out);
    out << "\n";
  }

  // member: confidence
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "confidence: ";
    rosidl_generator_traits::value_to_yaml(msg.confidence, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const SemanticDetection & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace semantic_msgs

namespace rosidl_generator_traits
{

[[deprecated("use semantic_msgs::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const semantic_msgs::msg::SemanticDetection & msg,
  std::ostream & out, size_t indentation = 0)
{
  semantic_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use semantic_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const semantic_msgs::msg::SemanticDetection & msg)
{
  return semantic_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<semantic_msgs::msg::SemanticDetection>()
{
  return "semantic_msgs::msg::SemanticDetection";
}

template<>
inline const char * name<semantic_msgs::msg::SemanticDetection>()
{
  return "semantic_msgs/msg/SemanticDetection";
}

template<>
struct has_fixed_size<semantic_msgs::msg::SemanticDetection>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<semantic_msgs::msg::SemanticDetection>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<semantic_msgs::msg::SemanticDetection>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // SEMANTIC_MSGS__MSG__DETAIL__SEMANTIC_DETECTION__TRAITS_HPP_
