from setuptools import find_packages, setup

package_name = 'baymax_brain'

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
    description='Nó cognitivo do Baymax (LLM local Qwen2.5 3B via Ollama).',
    license='MIT',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'brain_node = baymax_brain.brain_node:main',
        ],
    },
)
