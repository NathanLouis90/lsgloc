// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from semantic_msgs:msg/SemanticDetection.idl
// generated code does not contain a copyright notice

#ifndef SEMANTIC_MSGS__MSG__DETAIL__SEMANTIC_DETECTION__BUILDER_HPP_
#define SEMANTIC_MSGS__MSG__DETAIL__SEMANTIC_DETECTION__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "semantic_msgs/msg/detail/semantic_detection__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace semantic_msgs
{

namespace msg
{

namespace builder
{

class Init_SemanticDetection_confidence
{
public:
  explicit Init_SemanticDetection_confidence(::semantic_msgs::msg::SemanticDetection & msg)
  : msg_(msg)
  {}
  ::semantic_msgs::msg::SemanticDetection confidence(::semantic_msgs::msg::SemanticDetection::_confidence_type arg)
  {
    msg_.confidence = std::move(arg);
    return std::move(msg_);
  }

private:
  ::semantic_msgs::msg::SemanticDetection msg_;
};

class Init_SemanticDetection_range
{
public:
  explicit Init_SemanticDetection_range(::semantic_msgs::msg::SemanticDetection & msg)
  : msg_(msg)
  {}
  Init_SemanticDetection_confidence range(::semantic_msgs::msg::SemanticDetection::_range_type arg)
  {
    msg_.range = std::move(arg);
    return Init_SemanticDetection_confidence(msg_);
  }

private:
  ::semantic_msgs::msg::SemanticDetection msg_;
};

class Init_SemanticDetection_bearing
{
public:
  explicit Init_SemanticDetection_bearing(::semantic_msgs::msg::SemanticDetection & msg)
  : msg_(msg)
  {}
  Init_SemanticDetection_range bearing(::semantic_msgs::msg::SemanticDetection::_bearing_type arg)
  {
    msg_.bearing = std::move(arg);
    return Init_SemanticDetection_range(msg_);
  }

private:
  ::semantic_msgs::msg::SemanticDetection msg_;
};

class Init_SemanticDetection_label
{
public:
  Init_SemanticDetection_label()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_SemanticDetection_bearing label(::semantic_msgs::msg::SemanticDetection::_label_type arg)
  {
    msg_.label = std::move(arg);
    return Init_SemanticDetection_bearing(msg_);
  }

private:
  ::semantic_msgs::msg::SemanticDetection msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::semantic_msgs::msg::SemanticDetection>()
{
  return semantic_msgs::msg::builder::Init_SemanticDetection_label();
}

}  // namespace semantic_msgs

#endif  // SEMANTIC_MSGS__MSG__DETAIL__SEMANTIC_DETECTION__BUILDER_HPP_
