// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Revive_Ack.proto

#ifndef PROTOBUF_Revive_5fAck_2eproto__INCLUDED
#define PROTOBUF_Revive_5fAck_2eproto__INCLUDED

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
void  protobuf_AddDesc_Revive_5fAck_2eproto();
void protobuf_AssignDesc_Revive_5fAck_2eproto();
void protobuf_ShutdownFile_Revive_5fAck_2eproto();

class Revive_Ack;

// ===================================================================

class Revive_Ack : public ::google::protobuf::Message {
 public:
  Revive_Ack();
  virtual ~Revive_Ack();

  Revive_Ack(const Revive_Ack& from);

  inline Revive_Ack& operator=(const Revive_Ack& from) {
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
  static const Revive_Ack& default_instance();

  void Swap(Revive_Ack* other);

  // implements Message ----------------------------------------------

  Revive_Ack* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Revive_Ack& from);
  void MergeFrom(const Revive_Ack& from);
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

  // optional int32 subdiamond = 2;
  inline bool has_subdiamond() const;
  inline void clear_subdiamond();
  static const int kSubdiamondFieldNumber = 2;
  inline ::google::protobuf::int32 subdiamond() const;
  inline void set_subdiamond(::google::protobuf::int32 value);

  // optional int32 diamond = 3;
  inline bool has_diamond() const;
  inline void clear_diamond();
  static const int kDiamondFieldNumber = 3;
  inline ::google::protobuf::int32 diamond() const;
  inline void set_diamond(::google::protobuf::int32 value);

  // optional int32 acktime = 4;
  inline bool has_acktime() const;
  inline void clear_acktime();
  static const int kAcktimeFieldNumber = 4;
  inline ::google::protobuf::int32 acktime() const;
  inline void set_acktime(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:protobuf.Revive_Ack)
 private:
  inline void set_has_msginfo();
  inline void clear_has_msginfo();
  inline void set_has_subdiamond();
  inline void clear_has_subdiamond();
  inline void set_has_diamond();
  inline void clear_has_diamond();
  inline void set_has_acktime();
  inline void clear_has_acktime();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::std::string* msginfo_;
  ::google::protobuf::int32 subdiamond_;
  ::google::protobuf::int32 diamond_;
  ::google::protobuf::int32 acktime_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];

  friend void  protobuf_AddDesc_Revive_5fAck_2eproto();
  friend void protobuf_AssignDesc_Revive_5fAck_2eproto();
  friend void protobuf_ShutdownFile_Revive_5fAck_2eproto();

  void InitAsDefaultInstance();
  static Revive_Ack* default_instance_;
};
// ===================================================================


// ===================================================================

// Revive_Ack

// optional string msgInfo = 1;
inline bool Revive_Ack::has_msginfo() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Revive_Ack::set_has_msginfo() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Revive_Ack::clear_has_msginfo() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Revive_Ack::clear_msginfo() {
  if (msginfo_ != &::google::protobuf::internal::kEmptyString) {
    msginfo_->clear();
  }
  clear_has_msginfo();
}
inline const ::std::string& Revive_Ack::msginfo() const {
  return *msginfo_;
}
inline void Revive_Ack::set_msginfo(const ::std::string& value) {
  set_has_msginfo();
  if (msginfo_ == &::google::protobuf::internal::kEmptyString) {
    msginfo_ = new ::std::string;
  }
  msginfo_->assign(value);
}
inline void Revive_Ack::set_msginfo(const char* value) {
  set_has_msginfo();
  if (msginfo_ == &::google::protobuf::internal::kEmptyString) {
    msginfo_ = new ::std::string;
  }
  msginfo_->assign(value);
}
inline void Revive_Ack::set_msginfo(const char* value, size_t size) {
  set_has_msginfo();
  if (msginfo_ == &::google::protobuf::internal::kEmptyString) {
    msginfo_ = new ::std::string;
  }
  msginfo_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* Revive_Ack::mutable_msginfo() {
  set_has_msginfo();
  if (msginfo_ == &::google::protobuf::internal::kEmptyString) {
    msginfo_ = new ::std::string;
  }
  return msginfo_;
}
inline ::std::string* Revive_Ack::release_msginfo() {
  clear_has_msginfo();
  if (msginfo_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = msginfo_;
    msginfo_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void Revive_Ack::set_allocated_msginfo(::std::string* msginfo) {
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

// optional int32 subdiamond = 2;
inline bool Revive_Ack::has_subdiamond() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Revive_Ack::set_has_subdiamond() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Revive_Ack::clear_has_subdiamond() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Revive_Ack::clear_subdiamond() {
  subdiamond_ = 0;
  clear_has_subdiamond();
}
inline ::google::protobuf::int32 Revive_Ack::subdiamond() const {
  return subdiamond_;
}
inline void Revive_Ack::set_subdiamond(::google::protobuf::int32 value) {
  set_has_subdiamond();
  subdiamond_ = value;
}

// optional int32 diamond = 3;
inline bool Revive_Ack::has_diamond() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void Revive_Ack::set_has_diamond() {
  _has_bits_[0] |= 0x00000004u;
}
inline void Revive_Ack::clear_has_diamond() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void Revive_Ack::clear_diamond() {
  diamond_ = 0;
  clear_has_diamond();
}
inline ::google::protobuf::int32 Revive_Ack::diamond() const {
  return diamond_;
}
inline void Revive_Ack::set_diamond(::google::protobuf::int32 value) {
  set_has_diamond();
  diamond_ = value;
}

// optional int32 acktime = 4;
inline bool Revive_Ack::has_acktime() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void Revive_Ack::set_has_acktime() {
  _has_bits_[0] |= 0x00000008u;
}
inline void Revive_Ack::clear_has_acktime() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void Revive_Ack::clear_acktime() {
  acktime_ = 0;
  clear_has_acktime();
}
inline ::google::protobuf::int32 Revive_Ack::acktime() const {
  return acktime_;
}
inline void Revive_Ack::set_acktime(::google::protobuf::int32 value) {
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

#endif  // PROTOBUF_Revive_5fAck_2eproto__INCLUDED