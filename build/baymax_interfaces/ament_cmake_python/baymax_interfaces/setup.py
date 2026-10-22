from setuptools import find_packages
from setuptools import setup

setup(
    name='baymax_interfaces',
    version='0.1.0',
    packages=find_packages(
        include=('baymax_interfaces', 'baymax_interfaces.*')),
)
