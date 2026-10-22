from setuptools import find_packages, setup

package_name = 'baymax_motor'

setup(
    name=package_name,
    version='0.1.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools', 'pyserial'],
    zip_safe=True,
    maintainer='icbaymax',
    maintainer_email='icbaymax@example.com',
    description='Ponte serial UART entre ROS2 e os MCUs (cabeça/ESP32, braço/Arduino).',
    license='MIT',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'motor_bridge_node = baymax_motor.motor_bridge_node:main',
        ],
    },
)
