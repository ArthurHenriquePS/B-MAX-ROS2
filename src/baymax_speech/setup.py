from setuptools import find_packages, setup

package_name = 'baymax_speech'

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
    description='Áudio do Baymax: captura (mic), STT (whisper.cpp) e TTS (Piper).',
    license='MIT',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'mic_node = baymax_speech.mic_node:main',
            'stt_node = baymax_speech.stt_node:main',
            'tts_node = baymax_speech.tts_node:main',
        ],
    },
)
