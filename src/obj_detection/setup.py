from setuptools import find_packages, setup

package_name = 'obj_detection'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='sensethreat',
    maintainer_email='crusader2adventureabjure@gmail.com',
    description='TODO: Package description',
    license='TODO: License declaration',
    extras_require={
        'test': [
            'pytest',
        ],
    },
    entry_points={
        'console_scripts': [
            f'yolo_node = {package_name}.yolo_detector:main',
            f'vlm_service_node = {package_name}.vlm_service_detector:main',
            f'vlm_streaming_node = {package_name}.vlm_streaming_detector:main'
        ],
    },
)
