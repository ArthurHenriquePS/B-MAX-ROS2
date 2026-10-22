from setuptools import find_packages, setup

package_name = 'baymax_vision'

setup(
    name=package_name,
    version='0.1.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='icbaymax',
    maintainer_email='icbaymax@example.com',
    description='Visão do Baymax: webcam + detecção de expressão facial.',
    license='MIT',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'vision_node = baymax_vision.vision_node:main',
        ],
    },
)
