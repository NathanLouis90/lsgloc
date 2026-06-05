// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from semantic_msgs:msg/SemanticDetectionArray.idl
// generated code does not contain a copyright notice

#ifndef SEMANTIC_MSGS__MSG__DETAIL__SEMANTIC_DETECTION_ARRAY__STRUCT_HPP_
#define SEMANTIC_MSGS__MSG__DETAIL__SEMANTIC_DETECTION_ARRAY__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.hpp"
// Member 'detections'
#include "semantic_msgs/msg/detail/semantic_detection__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__semantic_msgs__msg__SemanticDetectionArray __attribute__((deprecated))
#else
# define DEPRECATED__semantic_msgs__msg__SemanticDetectionArray __declspec(deprecated)
#endif

namespace semantic_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct SemanticDetectionArray_
{
  using Type = SemanticDetectionArray_<ContainerAllocator>;

  explicit SemanticDetectionArray_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init)
  {
    (void)_init;
  }

  explicit SemanticDetectionArray_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init)
  {
    (void)_init;
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _detections_type =
    std::vector<semantic_msgs::msg::SemanticDetection_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<semantic_msgs::msg::SemanticDetection_<ContainerAllocator>>>;
  _detections_type detections;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__detections(
    const std::vector<semantic_msgs::msg::SemanticDetection_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<semantic_msgs::msg::SemanticDetection_<ContainerAllocator>>> & _arg)
  {
    this->detections = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    semantic_msgs::msg::SemanticDetectionArray_<ContainerAllocator> *;
  using ConstRawPtr =
    const semantic_msgs::msg::SemanticDetectionArray_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<semantic_msgs::msg::SemanticDetectionArray_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<semantic_msgs::msg::SemanticDetectionArray_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      semantic_msgs::msg::SemanticDetectionArray_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<semantic_msgs::msg::SemanticDetectionArray_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      semantic_msgs::msg::SemanticDetectionArray_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<semantic_msgs::msg::SemanticDetectionArray_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<semantic_msgs::msg::SemanticDetectionArray_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<semantic_msgs::msg::SemanticDetectionArray_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__semantic_msgs__msg__SemanticDetectionArray
    std::shared_ptr<semantic_msgs::msg::SemanticDetectionArray_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__semantic_msgs__msg__SemanticDetectionArray
    std::shared_ptr<semantic_msgs::msg::SemanticDetectionArray_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const SemanticDetectionArray_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->detections != other.detections) {
      return false;
    }
    return true;
  }
  bool operator!=(const SemanticDetectionArray_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct SemanticDetectionArray_

// alias to use template instance with default allocator
using SemanticDetectionArray =
  semantic_msgs::msg::SemanticDetectionArray_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace semantic_msgs

#endif  // SEMANTIC_MSGS__MSG__DETAIL__SEMANTIC_DETECTION_ARRAY__STRUCT_HPP_
