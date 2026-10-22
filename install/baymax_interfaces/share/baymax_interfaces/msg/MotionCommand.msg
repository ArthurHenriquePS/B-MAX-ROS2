# Comando de movimento emitido pelo brain_node para o motor_bridge_node.
#
# target: qual MCU/subsistema. Um de {head, arm}
string target

# kind: tipo de comando. Um de {pan, tilt, head_gesture,
#       shoulder, elbow, grip, arm_gesture, stop, torque}
string kind

# value: ângulo (0-180), garra (0-100) ou torque (0/1), quando numérico
int32 value

# name: nome do gesto (ex. "nod", "wave"), quando kind for *_gesture
string name
