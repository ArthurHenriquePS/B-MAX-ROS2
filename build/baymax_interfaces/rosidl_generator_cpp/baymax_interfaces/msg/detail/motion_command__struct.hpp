// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from baymax_interfaces:msg/MotionCommand.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "baymax_interfaces/msg/motion_command.hpp"


#ifndef BAYMAX_INTERFACES__MSG__DETAIL__MOTION_COMMAND__STRUCT_HPP_
#define BAYMAX_INTERFACES__MSG__DETAIL__MOTION_COMMAND__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__baymax_interfaces__msg__MotionCommand __attribute__((deprecated))
#else
# define DEPRECATED__baymax_interfaces__msg__MotionCommand __declspec(deprecated)
#endif

namespace baymax_interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct MotionCommand_
{
  using Type = MotionCommand_<ContainerAllocator>;

  explicit MotionCommand_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->target = "";
      this->kind = "";
      this->value = 0l;
      this->name = "";
    }
  }

  explicit MotionCommand_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : target(_alloc),
    kind(_alloc),
    name(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->target = "";
      this->kind = "";
      this->value = 0l;
      this->name = "";
    }
  }

  // field types and members
  using _target_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _target_type target;
  using _kind_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _kind_type kind;
  using _value_type =
    int32_t;
  _value_type value;
  using _name_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _name_type name;

  // setters for named parameter idiom
  Type & set__target(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->target = _arg;
    return *this;
  }
  Type & set__kind(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->kind = _arg;
    return *this;
  }
  Type & set__value(
    const int32_t & _arg)
  {
    this->value = _arg;
    return *this;
  }
  Type & set__name(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->name = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    baymax_interfaces::msg::MotionCommand_<ContainerAllocator> *;
  using ConstRawPtr =
    const baymax_interfaces::msg::MotionCommand_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<baymax_interfaces::msg::MotionCommand_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<baymax_interfaces::msg::MotionCommand_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      baymax_interfaces::msg::MotionCommand_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<baymax_interfaces::msg::MotionCommand_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      baymax_interfaces::msg::MotionCommand_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<baymax_interfaces::msg::MotionCommand_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<baymax_interfaces::msg::MotionCommand_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<baymax_interfaces::msg::MotionCommand_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__baymax_interfaces__msg__MotionCommand
    std::shared_ptr<baymax_interfaces::msg::MotionCommand_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__baymax_interfaces__msg__MotionCommand
    std::shared_ptr<baymax_interfaces::msg::MotionCommand_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const MotionCommand_ & other) const
  {
    if (this->target != other.target) {
      return false;
    }
    if (this->kind != other.kind) {
      return false;
    }
    if (this->value != other.value) {
      return false;
    }
    if (this->name != other.name) {
      return false;
    }
    return true;
  }
  bool operator!=(const MotionCommand_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct MotionCommand_

// alias to use template instance with default allocator
using MotionCommand =
  baymax_interfaces::msg::MotionCommand_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace baymax_interfaces

#endif  // BAYMAX_INTERFACES__MSG__DETAIL__MOTION_COMMAND__STRUCT_HPP_
