"""Launch de bringup do Baymax: sobe todos os nós do cérebro.

Uso:
  ros2 launch baymax_bringup baymax.launch.py
  ros2 launch baymax_bringup baymax.launch.py simulate:=false head_port:=/dev/baymax_head

Argumentos:
  simulate    (true/false)  motor_bridge em simulação (sem hardware). Default true.
  head_port   porta serial da cabeça/ESP32.  Default /dev/ttyUSB0
  arm_port    porta serial do braço/Arduino. Default /dev/ttyUSB1
  model       modelo Ollama do brain_node.   Default qwen2.5:3b
  speech_venv caminho do venv com Piper/whisper/áudio.
              Default: <workspace>/.venv-speech. Os nós de fala (tts/stt/mic) rodam
              com esse interpretador, que vê tanto o ROS (rclpy) quanto o piper.

Nota: os nós de fala precisam do venv .venv-speech (Piper etc.). O launch invoca
esses nós com o python do venv via 'prefix'. Os demais nós usam o python do sistema.
"""
import os

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration, PythonExpression
from launch_ros.actions import Node


def generate_launch_description():
    simulate = LaunchConfiguration("simulate")
    head_port = LaunchConfiguration("head_port")
    arm_port = LaunchConfiguration("arm_port")
    model = LaunchConfiguration("model")
    speech_venv = LaunchConfiguration("speech_venv")

    default_venv = os.path.expanduser("~/baymax_ws/.venv-speech")

    # prefixo que faz o nó rodar com o python do venv de fala
    speech_prefix = PythonExpression(
        ["'", speech_venv, "/bin/python3'"]
    )

    return LaunchDescription([
        DeclareLaunchArgument("simulate", default_value="true"),
        DeclareLaunchArgument("head_port", default_value="/dev/ttyUSB0"),
        DeclareLaunchArgument("arm_port", default_value="/dev/ttyUSB1"),
        DeclareLaunchArgument("model", default_value="qwen2.5:3b"),
        DeclareLaunchArgument("speech_venv", default_value=default_venv),

        Node(
            package="baymax_brain", executable="brain_node", name="brain_node",
            output="screen",
            parameters=[{"model": model}],
        ),
        Node(
            package="baymax_speech", executable="stt_node", name="stt_node",
            output="screen",
            prefix=speech_prefix,
            parameters=[{
                "model": "base",
                "models_dir": os.path.expanduser("~/baymax_ws/models/whisper"),
                "language": "pt",
                "n_threads": 8,
            }],
        ),
        Node(
            package="baymax_speech", executable="tts_node", name="tts_node",
            output="screen",
            prefix=speech_prefix,
            parameters=[{
                "voice": os.path.expanduser(
                    "~/baymax_ws/models/pt_BR-faber-medium.onnx"
                ),
                "audio_device": "plughw:1,0",
            }],
        ),
        Node(
            package="baymax_speech", executable="mic_node", name="mic_node",
            output="screen",
            prefix=speech_prefix,
        ),
        Node(
            package="baymax_vision", executable="vision_node", name="vision_node",
            output="screen",
            parameters=[{"enabled": False}],
        ),
        Node(
            package="baymax_motor", executable="motor_bridge_node",
            name="motor_bridge_node", output="screen",
            parameters=[{
                "simulate": simulate,
                "head_port": head_port,
                "arm_port": arm_port,
            }],
        ),
    ])
