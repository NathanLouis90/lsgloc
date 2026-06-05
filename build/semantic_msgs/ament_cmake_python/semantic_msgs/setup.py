from setuptools import find_packages
from setuptools import setup

setup(
    name='semantic_msgs',
    version='0.0.0',
    packages=find_packages(
        include=('semantic_msgs', 'semantic_msgs.*')),
)
