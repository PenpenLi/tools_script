// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: DelFriend_Ack.proto

#ifndef PROTOBUF_DelFriend_5fAck_2eproto__INCLUDED
#define PROTOBUF_DelFriend_5fAck_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2005000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace protobuf {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_DelFriend_5fAck_2eproto();
void protobuf_AssignDesc_DelFriend_5fAck_2eproto();
void protobuf_ShutdownFile_DelFriend_5fAck_2eproto();

class DelFriend_Ack;

// ===================================================================

class DelFriend_Ack : public ::google::protobuf::Message {
 public:
  DelFriend_Ack();
  virtual ~DelFriend_Ack();

  DelFriend_Ack(const DelFriend_Ack& from);

  inline DelFriend_Ack& operator=(const DelFriend_Ack& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const DelFriend_Ack& default_instance();

  void Swap(DelFriend_Ack* other);

  // implements Message ----------------------------------------------

  DelFriend_Ack* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const DelFriend_Ack& from);
  void MergeFrom(const DelFriend_Ack& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional string msgInfo = 1;
  inline bool has_msginfo() const;
  inline void clear_msginfo();
  static const int kMsgInfoFieldNumber = 1;
  inline const ::std::string& msginfo() const;
  inline void set_msginfo(const ::std::string& value);
  inline void set_msginfo(const char* value);
  inline void set_msginfo(const char* value, size_t size);
  inline ::std::string* mutable_msginfo();
  inline ::std::string* release_msginfo();
  inline void set_allocated_msginfo(::std::string* msginfo);

  // required int32 acktime = 2;
  inline bool has_acktime() const;
  inline void clear_acktime();
  static const int kAcktimeFieldNumber = 2;
  inline ::google::protobuf::int32 acktime() const;
  inline void set_acktime(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:protobuf.DelFriend_Ack)
 private:
  inline void set_has_msginfo();
  inline void clear_has_msginfo();
  inline void set_has_acktime();
  inline void clear_has_acktime();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::std::string* msginfo_;
  ::google::protobuf::int32 acktime_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  friend void  protobuf_AddDesc_DelFriend_5fAck_2eproto();
  friend void protobuf_AssignDesc_DelFriend_5fAck_2eproto();
  friend void protobuf_ShutdownFile_DelFriend_5fAck_2eproto();

  void InitAsDefaultInstance();
  static DelFriend_Ack* default_instance_;
};
// ===================================================================


// ===================================================================

// DelFriend_Ack

// optional string msgInfo = 1;
inline bool DelFriend_Ack::has_msginfo() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void DelFriend_Ack::set_has_msginfo() {
  _has_bits_[0] |= 0x00000001u;
}
inline void DelFriend_Ack::clear_has_msginfo() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void DelFriend_Ack::clear_msginfo() {
  if (msginfo_ != &::google::protobuf::internal::kEmptyString) {
    msginfo_->clear();
  }
  clear_has_msginfo();
}
inline const ::std::string& DelFriend_Ack::msginfo() const {
  return *msginfo_;
}
inline void DelFriend_Ack::set_msginfo(const ::std::string& value) {
  set_has_msginfo();
  if (msginfo_ == &::google::protobuf::internal::kEmptyString) {
    msginfo_ = new ::std::string;
  }
  msginfo_->assign(value);
}
inline void DelFriend_Ack::set_msginfo(const char* value) {
  set_has_msginfo();
  if (msginfo_ == &::google::protobuf::internal::kEmptyString) {
    msginfo_ = new ::std::string;
  }
  msginfo_->assign(value);
}
inline void DelFriend_Ack::set_msginfo(const char* value, size_t size) {
  set_has_msginfo();
  if (msginfo_ == &::google::protobuf::internal::kEmptyString) {
    msginfo_ = new ::std::string;
  }
  msginfo_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* DelFriend_Ack::mutable_msginfo() {
  set_has_msginfo();
  if (msginfo_ == &::google::protobuf::internal::kEmptyString) {
    msginfo_ = new ::std::string;
  }
  return msginfo_;
}
inline ::std::string* DelFriend_Ack::release_msginfo() {
  clear_has_msginfo();
  if (msginfo_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = msginfo_;
    msginfo_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void DelFriend_Ack::set_allocated_msginfo(::std::string* msginfo) {
  if (msginfo_ != &::google::protobuf::internal::kEmptyString) {
    delete msginfo_;
  }
  if (msginfo) {
    set_has_msginfo();
    msginfo_ = msginfo;
  } else {
    clear_has_msginfo();
    msginfo_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// required int32 acktime = 2;
inline bool DelFriend_Ack::has_acktime() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void DelFriend_Ack::set_has_acktime() {
  _has_bits_[0] |= 0x00000002u;
}
inline void DelFriend_Ack::clear_has_acktime() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void DelFriend_Ack::clear_acktime() {
  acktime_ = 0;
  clear_has_acktime();
}
inline ::google::protobuf::int32 DelFriend_Ack::acktime() const {
  return acktime_;
}
inline void DelFriend_Ack::set_acktime(::google::protobuf::int32 value) {
  set_has_acktime();
  acktime_ = value;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace protobuf

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_DelFriend_5fAck_2eproto__INCLUDED
