#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};


#[link(name = "baymax_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__baymax_interfaces__msg__Emotion() -> *const std::ffi::c_void;
}

#[link(name = "baymax_interfaces__rosidl_generator_c")]
extern "C" {
    fn baymax_interfaces__msg__Emotion__init(msg: *mut Emotion) -> bool;
    fn baymax_interfaces__msg__Emotion__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<Emotion>, size: usize) -> bool;
    fn baymax_interfaces__msg__Emotion__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<Emotion>);
    fn baymax_interfaces__msg__Emotion__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<Emotion>, out_seq: *mut rosidl_runtime_rs::Sequence<Emotion>) -> bool;
}

// Corresponds to baymax_interfaces__msg__Emotion
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]

/// Expressão/emoção detectada pelo vision_node.
/// label: uma de {neutral, happy, sad, angry, surprised, fearful, disgusted}

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct Emotion {

    // This member is not documented.
    #[allow(missing_docs)]
    pub label: rosidl_runtime_rs::String,

    /// confidence: 0.0 a 1.0
    pub confidence: f32,

    /// timestamp da captura
    pub stamp: builtin_interfaces::msg::rmw::Time,

}



impl Default for Emotion {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !baymax_interfaces__msg__Emotion__init(&mut msg as *mut _) {
        panic!("Call to baymax_interfaces__msg__Emotion__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for Emotion {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { baymax_interfaces__msg__Emotion__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { baymax_interfaces__msg__Emotion__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { baymax_interfaces__msg__Emotion__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for Emotion {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for Emotion where Self: Sized {
  const TYPE_NAME: &'static str = "baymax_interfaces/msg/Emotion";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__baymax_interfaces__msg__Emotion() }
  }
}


#[link(name = "baymax_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__baymax_interfaces__msg__MotionCommand() -> *const std::ffi::c_void;
}

#[link(name = "baymax_interfaces__rosidl_generator_c")]
extern "C" {
    fn baymax_interfaces__msg__MotionCommand__init(msg: *mut MotionCommand) -> bool;
    fn baymax_interfaces__msg__MotionCommand__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<MotionCommand>, size: usize) -> bool;
    fn baymax_interfaces__msg__MotionCommand__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<MotionCommand>);
    fn baymax_interfaces__msg__MotionCommand__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<MotionCommand>, out_seq: *mut rosidl_runtime_rs::Sequence<MotionCommand>) -> bool;
}

// Corresponds to baymax_interfaces__msg__MotionCommand
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]

/// Comando de movimento emitido pelo brain_node para o motor_bridge_node.
///
/// target: qual MCU/subsistema. Um de {head, arm}

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct MotionCommand {

    // This member is not documented.
    #[allow(missing_docs)]
    pub target: rosidl_runtime_rs::String,

    /// kind: tipo de comando. Um de {pan, tilt, head_gesture,
    ///       shoulder, elbow, grip, arm_gesture, stop, torque}
    pub kind: rosidl_runtime_rs::String,

    /// value: ângulo (0-180), garra (0-100) ou torque (0/1), quando numérico
    pub value: i32,

    /// name: nome do gesto (ex. "nod", "wave"), quando kind for *_gesture
    pub name: rosidl_runtime_rs::String,

}



impl Default for MotionCommand {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !baymax_interfaces__msg__MotionCommand__init(&mut msg as *mut _) {
        panic!("Call to baymax_interfaces__msg__MotionCommand__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for MotionCommand {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { baymax_interfaces__msg__MotionCommand__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { baymax_interfaces__msg__MotionCommand__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { baymax_interfaces__msg__MotionCommand__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for MotionCommand {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for MotionCommand where Self: Sized {
  const TYPE_NAME: &'static str = "baymax_interfaces/msg/MotionCommand";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__baymax_interfaces__msg__MotionCommand() }
  }
}


