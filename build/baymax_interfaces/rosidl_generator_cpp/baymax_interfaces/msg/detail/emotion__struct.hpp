// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from baymax_interfaces:msg/Emotion.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "baymax_interfaces/msg/emotion.hpp"


#ifndef BAYMAX_INTERFACES__MSG__DETAIL__EMOTION__STRUCT_HPP_
#define BAYMAX_INTERFACES__MSG__DETAIL__EMOTION__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'stamp'
#include "builtin_interfaces/msg/detail/time__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__baymax_interfaces__msg__Emotion __attribute__((deprecated))
#else
# define DEPRECATED__baymax_interfaces__msg__Emotion __declspec(deprecated)
#endif

namespace baymax_interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct Emotion_
{
  using Type = Emotion_<ContainerAllocator>;

  explicit Emotion_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : stamp(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->label = "";
      this->confidence = 0.0f;
    }
  }

  explicit Emotion_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : label(_alloc),
    stamp(_alloc, _init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->label = "";
      this->confidence = 0.0f;
    }
  }

  // field types and members
  using _label_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _label_type label;
  using _confidence_type =
    float;
  _confidence_type confidence;
  using _stamp_type =
    builtin_interfaces::msg::Time_<ContainerAllocator>;
  _stamp_type stamp;

  // setters for named parameter idiom
  Type & set__label(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->label = _arg;
    return *this;
  }
  Type & set__confidence(
    const float & _arg)
  {
    this->confidence = _arg;
    return *this;
  }
  Type & set__stamp(
    const builtin_interfaces::msg::Time_<ContainerAllocator> & _arg)
  {
    this->stamp = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    baymax_interfaces::msg::Emotion_<ContainerAllocator> *;
  using ConstRawPtr =
    const baymax_interfaces::msg::Emotion_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<baymax_interfaces::msg::Emotion_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<baymax_interfaces::msg::Emotion_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      baymax_interfaces::msg::Emotion_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<baymax_interfaces::msg::Emotion_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      baymax_interfaces::msg::Emotion_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<baymax_interfaces::msg::Emotion_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<baymax_interfaces::msg::Emotion_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<baymax_interfaces::msg::Emotion_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__baymax_interfaces__msg__Emotion
    std::shared_ptr<baymax_interfaces::msg::Emotion_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__baymax_interfaces__msg__Emotion
    std::shared_ptr<baymax_interfaces::msg::Emotion_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Emotion_ & other) const
  {
    if (this->label != other.label) {
      return false;
    }
    if (this->confidence != other.confidence) {
      return false;
    }
    if (this->stamp != other.stamp) {
      return false;
    }
    return true;
  }
  bool operator!=(const Emotion_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Emotion_

// alias to use template instance with default allocator
using Emotion =
  baymax_interfaces::msg::Emotion_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace baymax_interfaces

#endif  // BAYMAX_INTERFACES__MSG__DETAIL__EMOTION__STRUCT_HPP_
