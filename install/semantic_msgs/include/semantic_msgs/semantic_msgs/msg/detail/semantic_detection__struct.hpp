// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from semantic_msgs:msg/SemanticDetection.idl
// generated code does not contain a copyright notice

#ifndef SEMANTIC_MSGS__MSG__DETAIL__SEMANTIC_DETECTION__STRUCT_HPP_
#define SEMANTIC_MSGS__MSG__DETAIL__SEMANTIC_DETECTION__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__semantic_msgs__msg__SemanticDetection __attribute__((deprecated))
#else
# define DEPRECATED__semantic_msgs__msg__SemanticDetection __declspec(deprecated)
#endif

namespace semantic_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct SemanticDetection_
{
  using Type = SemanticDetection_<ContainerAllocator>;

  explicit SemanticDetection_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->label = "";
      this->bearing = 0.0;
      this->range = 0.0;
      this->confidence = 0.0;
    }
  }

  explicit SemanticDetection_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : label(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->label = "";
      this->bearing = 0.0;
      this->range = 0.0;
      this->confidence = 0.0;
    }
  }

  // field types and members
  using _label_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _label_type label;
  using _bearing_type =
    double;
  _bearing_type bearing;
  using _range_type =
    double;
  _range_type range;
  using _confidence_type =
    double;
  _confidence_type confidence;

  // setters for named parameter idiom
  Type & set__label(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->label = _arg;
    return *this;
  }
  Type & set__bearing(
    const double & _arg)
  {
    this->bearing = _arg;
    return *this;
  }
  Type & set__range(
    const double & _arg)
  {
    this->range = _arg;
    return *this;
  }
  Type & set__confidence(
    const double & _arg)
  {
    this->confidence = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    semantic_msgs::msg::SemanticDetection_<ContainerAllocator> *;
  using ConstRawPtr =
    const semantic_msgs::msg::SemanticDetection_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<semantic_msgs::msg::SemanticDetection_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<semantic_msgs::msg::SemanticDetection_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      semantic_msgs::msg::SemanticDetection_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<semantic_msgs::msg::SemanticDetection_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      semantic_msgs::msg::SemanticDetection_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<semantic_msgs::msg::SemanticDetection_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<semantic_msgs::msg::SemanticDetection_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<semantic_msgs::msg::SemanticDetection_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__semantic_msgs__msg__SemanticDetection
    std::shared_ptr<semantic_msgs::msg::SemanticDetection_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__semantic_msgs__msg__SemanticDetection
    std::shared_ptr<semantic_msgs::msg::SemanticDetection_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const SemanticDetection_ & other) const
  {
    if (this->label != other.label) {
      return false;
    }
    if (this->bearing != other.bearing) {
      return false;
    }
    if (this->range != other.range) {
      return false;
    }
    if (this->confidence != other.confidence) {
      return false;
    }
    return true;
  }
  bool operator!=(const SemanticDetection_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct SemanticDetection_

// alias to use template instance with default allocator
using SemanticDetection =
  semantic_msgs::msg::SemanticDetection_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace semantic_msgs

#endif  // SEMANTIC_MSGS__MSG__DETAIL__SEMANTIC_DETECTION__STRUCT_HPP_
