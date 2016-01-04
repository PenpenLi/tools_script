// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ItemList_Ack.proto

#ifndef PROTOBUF_ItemList_5fAck_2eproto__INCLUDED
#define PROTOBUF_ItemList_5fAck_2eproto__INCLUDED

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
void  protobuf_AddDesc_ItemList_5fAck_2eproto();
void protobuf_AssignDesc_ItemList_5fAck_2eproto();
void protobuf_ShutdownFile_ItemList_5fAck_2eproto();

class ItemList_Ack;
class ItemList_Ack_ItemList;

// ===================================================================

class ItemList_Ack_ItemList : public ::google::protobuf::Message {
 public:
  ItemList_Ack_ItemList();
  virtual ~ItemList_Ack_ItemList();

  ItemList_Ack_ItemList(const ItemList_Ack_ItemList& from);

  inline ItemList_Ack_ItemList& operator=(const ItemList_Ack_ItemList& from) {
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
  static const ItemList_Ack_ItemList& default_instance();

  void Swap(ItemList_Ack_ItemList* other);

  // implements Message ----------------------------------------------

  ItemList_Ack_ItemList* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ItemList_Ack_ItemList& from);
  void MergeFrom(const ItemList_Ack_ItemList& from);
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

  // optional int32 itemid = 1;
  inline bool has_itemid() const;
  inline void clear_itemid();
  static const int kItemidFieldNumber = 1;
  inline ::google::protobuf::int32 itemid() const;
  inline void set_itemid(::google::protobuf::int32 value);

  // optional int32 count = 2;
  inline bool has_count() const;
  inline void clear_count();
  static const int kCountFieldNumber = 2;
  inline ::google::protobuf::int32 count() const;
  inline void set_count(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:protobuf.ItemList_Ack.ItemList)
 private:
  inline void set_has_itemid();
  inline void clear_has_itemid();
  inline void set_has_count();
  inline void clear_has_count();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int32 itemid_;
  ::google::protobuf::int32 count_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  friend void  protobuf_AddDesc_ItemList_5fAck_2eproto();
  friend void protobuf_AssignDesc_ItemList_5fAck_2eproto();
  friend void protobuf_ShutdownFile_ItemList_5fAck_2eproto();

  void InitAsDefaultInstance();
  static ItemList_Ack_ItemList* default_instance_;
};
// -------------------------------------------------------------------

class ItemList_Ack : public ::google::protobuf::Message {
 public:
  ItemList_Ack();
  virtual ~ItemList_Ack();

  ItemList_Ack(const ItemList_Ack& from);

  inline ItemList_Ack& operator=(const ItemList_Ack& from) {
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
  static const ItemList_Ack& default_instance();

  void Swap(ItemList_Ack* other);

  // implements Message ----------------------------------------------

  ItemList_Ack* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ItemList_Ack& from);
  void MergeFrom(const ItemList_Ack& from);
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

  typedef ItemList_Ack_ItemList ItemList;

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

  // repeated .protobuf.ItemList_Ack.ItemList itemlist = 2;
  inline int itemlist_size() const;
  inline void clear_itemlist();
  static const int kItemlistFieldNumber = 2;
  inline const ::protobuf::ItemList_Ack_ItemList& itemlist(int index) const;
  inline ::protobuf::ItemList_Ack_ItemList* mutable_itemlist(int index);
  inline ::protobuf::ItemList_Ack_ItemList* add_itemlist();
  inline const ::google::protobuf::RepeatedPtrField< ::protobuf::ItemList_Ack_ItemList >&
      itemlist() const;
  inline ::google::protobuf::RepeatedPtrField< ::protobuf::ItemList_Ack_ItemList >*
      mutable_itemlist();

  // optional int32 ItemId = 3;
  inline bool has_itemid() const;
  inline void clear_itemid();
  static const int kItemIdFieldNumber = 3;
  inline ::google::protobuf::int32 itemid() const;
  inline void set_itemid(::google::protobuf::int32 value);

  // optional int32 acktime = 4;
  inline bool has_acktime() const;
  inline void clear_acktime();
  static const int kAcktimeFieldNumber = 4;
  inline ::google::protobuf::int32 acktime() const;
  inline void set_acktime(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:protobuf.ItemList_Ack)
 private:
  inline void set_has_msginfo();
  inline void clear_has_msginfo();
  inline void set_has_itemid();
  inline void clear_has_itemid();
  inline void set_has_acktime();
  inline void clear_has_acktime();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::std::string* msginfo_;
  ::google::protobuf::RepeatedPtrField< ::protobuf::ItemList_Ack_ItemList > itemlist_;
  ::google::protobuf::int32 itemid_;
  ::google::protobuf::int32 acktime_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];

  friend void  protobuf_AddDesc_ItemList_5fAck_2eproto();
  friend void protobuf_AssignDesc_ItemList_5fAck_2eproto();
  friend void protobuf_ShutdownFile_ItemList_5fAck_2eproto();

  void InitAsDefaultInstance();
  static ItemList_Ack* default_instance_;
};
// ===================================================================


// ===================================================================

// ItemList_Ack_ItemList

// optional int32 itemid = 1;
inline bool ItemList_Ack_ItemList::has_itemid() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ItemList_Ack_ItemList::set_has_itemid() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ItemList_Ack_ItemList::clear_has_itemid() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ItemList_Ack_ItemList::clear_itemid() {
  itemid_ = 0;
  clear_has_itemid();
}
inline ::google::protobuf::int32 ItemList_Ack_ItemList::itemid() const {
  return itemid_;
}
inline void ItemList_Ack_ItemList::set_itemid(::google::protobuf::int32 value) {
  set_has_itemid();
  itemid_ = value;
}

// optional int32 count = 2;
inline bool ItemList_Ack_ItemList::has_count() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ItemList_Ack_ItemList::set_has_count() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ItemList_Ack_ItemList::clear_has_count() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ItemList_Ack_ItemList::clear_count() {
  count_ = 0;
  clear_has_count();
}
inline ::google::protobuf::int32 ItemList_Ack_ItemList::count() const {
  return count_;
}
inline void ItemList_Ack_ItemList::set_count(::google::protobuf::int32 value) {
  set_has_count();
  count_ = value;
}

// -------------------------------------------------------------------

// ItemList_Ack

// optional string msgInfo = 1;
inline bool ItemList_Ack::has_msginfo() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ItemList_Ack::set_has_msginfo() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ItemList_Ack::clear_has_msginfo() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ItemList_Ack::clear_msginfo() {
  if (msginfo_ != &::google::protobuf::internal::kEmptyString) {
    msginfo_->clear();
  }
  clear_has_msginfo();
}
inline const ::std::string& ItemList_Ack::msginfo() const {
  return *msginfo_;
}
inline void ItemList_Ack::set_msginfo(const ::std::string& value) {
  set_has_msginfo();
  if (msginfo_ == &::google::protobuf::internal::kEmptyString) {
    msginfo_ = new ::std::string;
  }
  msginfo_->assign(value);
}
inline void ItemList_Ack::set_msginfo(const char* value) {
  set_has_msginfo();
  if (msginfo_ == &::google::protobuf::internal::kEmptyString) {
    msginfo_ = new ::std::string;
  }
  msginfo_->assign(value);
}
inline void ItemList_Ack::set_msginfo(const char* value, size_t size) {
  set_has_msginfo();
  if (msginfo_ == &::google::protobuf::internal::kEmptyString) {
    msginfo_ = new ::std::string;
  }
  msginfo_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* ItemList_Ack::mutable_msginfo() {
  set_has_msginfo();
  if (msginfo_ == &::google::protobuf::internal::kEmptyString) {
    msginfo_ = new ::std::string;
  }
  return msginfo_;
}
inline ::std::string* ItemList_Ack::release_msginfo() {
  clear_has_msginfo();
  if (msginfo_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = msginfo_;
    msginfo_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void ItemList_Ack::set_allocated_msginfo(::std::string* msginfo) {
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

// repeated .protobuf.ItemList_Ack.ItemList itemlist = 2;
inline int ItemList_Ack::itemlist_size() const {
  return itemlist_.size();
}
inline void ItemList_Ack::clear_itemlist() {
  itemlist_.Clear();
}
inline const ::protobuf::ItemList_Ack_ItemList& ItemList_Ack::itemlist(int index) const {
  return itemlist_.Get(index);
}
inline ::protobuf::ItemList_Ack_ItemList* ItemList_Ack::mutable_itemlist(int index) {
  return itemlist_.Mutable(index);
}
inline ::protobuf::ItemList_Ack_ItemList* ItemList_Ack::add_itemlist() {
  return itemlist_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::protobuf::ItemList_Ack_ItemList >&
ItemList_Ack::itemlist() const {
  return itemlist_;
}
inline ::google::protobuf::RepeatedPtrField< ::protobuf::ItemList_Ack_ItemList >*
ItemList_Ack::mutable_itemlist() {
  return &itemlist_;
}

// optional int32 ItemId = 3;
inline bool ItemList_Ack::has_itemid() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void ItemList_Ack::set_has_itemid() {
  _has_bits_[0] |= 0x00000004u;
}
inline void ItemList_Ack::clear_has_itemid() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void ItemList_Ack::clear_itemid() {
  itemid_ = 0;
  clear_has_itemid();
}
inline ::google::protobuf::int32 ItemList_Ack::itemid() const {
  return itemid_;
}
inline void ItemList_Ack::set_itemid(::google::protobuf::int32 value) {
  set_has_itemid();
  itemid_ = value;
}

// optional int32 acktime = 4;
inline bool ItemList_Ack::has_acktime() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void ItemList_Ack::set_has_acktime() {
  _has_bits_[0] |= 0x00000008u;
}
inline void ItemList_Ack::clear_has_acktime() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void ItemList_Ack::clear_acktime() {
  acktime_ = 0;
  clear_has_acktime();
}
inline ::google::protobuf::int32 ItemList_Ack::acktime() const {
  return acktime_;
}
inline void ItemList_Ack::set_acktime(::google::protobuf::int32 value) {
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

#endif  // PROTOBUF_ItemList_5fAck_2eproto__INCLUDED
