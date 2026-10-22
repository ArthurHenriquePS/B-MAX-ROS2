#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};



// Corresponds to baymax_interfaces__msg__Emotion
/// Expressão/emoção detectada pelo vision_node.
/// label: uma de {neutral, happy, sad, angry, surprised, fearful, disgusted}

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct Emotion {

    // This member is not documented.
    #[allow(missing_docs)]
    pub label: std::string::String,

    /// confidence: 0.0 a 1.0
    pub confidence: f32,

    /// timestamp da captura
    pub stamp: builtin_interfaces::msg::Time,

}



impl Default for Emotion {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::Emotion::default())
  }
}

impl rosidl_runtime_rs::Message for Emotion {
  type RmwMsg = super::msg::rmw::Emotion;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        label: msg.label.as_str().into(),
        confidence: msg.confidence,
        stamp: builtin_interfaces::msg::Time::into_rmw_message(std::borrow::Cow::Owned(msg.stamp)).into_owned(),
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        label: msg.label.as_str().into(),
      confidence: msg.confidence,
        stamp: builtin_interfaces::msg::Time::into_rmw_message(std::borrow::Cow::Borrowed(&msg.stamp)).into_owned(),
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      label: msg.label.to_string(),
      confidence: msg.confidence,
      stamp: builtin_interfaces::msg::Time::from_rmw_message(msg.stamp),
    }
  }
}


// Corresponds to baymax_interfaces__msg__MotionCommand
/// Comando de movimento emitido pelo brain_node para o motor_bridge_node.
///
/// target: qual MCU/subsistema. Um de {head, arm}

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct MotionCommand {

    // This member is not documented.
    #[allow(missing_docs)]
    pub target: std::string::String,

    /// kind: tipo de comando. Um de {pan, tilt, head_gesture,
    ///       shoulder, elbow, grip, arm_gesture, stop, torque}
    pub kind: std::string::String,

    /// value: ângulo (0-180), garra (0-100) ou torque (0/1), quando numérico
    pub value: i32,

    /// name: nome do gesto (ex. "nod", "wave"), quando kind for *_gesture
    pub name: std::string::String,

}



impl Default for MotionCommand {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::MotionCommand::default())
  }
}

impl rosidl_runtime_rs::Message for MotionCommand {
  type RmwMsg = super::msg::rmw::MotionCommand;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        target: msg.target.as_str().into(),
        kind: msg.kind.as_str().into(),
        value: msg.value,
        name: msg.name.as_str().into(),
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        target: msg.target.as_str().into(),
        kind: msg.kind.as_str().into(),
      value: msg.value,
        name: msg.name.as_str().into(),
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      target: msg.target.to_string(),
      kind: msg.kind.to_string(),
      value: msg.value,
      name: msg.name.to_string(),
    }
  }
}


