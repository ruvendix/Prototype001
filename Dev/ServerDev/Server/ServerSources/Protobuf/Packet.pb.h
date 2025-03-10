// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Packet.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_Packet_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_Packet_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3021000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3021003 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
#include "Enum.pb.h"
#include "Struct.pb.h"
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_Packet_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_Packet_2eproto {
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_Packet_2eproto;
namespace Protocol {
class S_CreateLocalGamePlayer;
struct S_CreateLocalGamePlayerDefaultTypeInternal;
extern S_CreateLocalGamePlayerDefaultTypeInternal _S_CreateLocalGamePlayer_default_instance_;
class S_EnterGame;
struct S_EnterGameDefaultTypeInternal;
extern S_EnterGameDefaultTypeInternal _S_EnterGame_default_instance_;
class S_SyncGameEntities;
struct S_SyncGameEntitiesDefaultTypeInternal;
extern S_SyncGameEntitiesDefaultTypeInternal _S_SyncGameEntities_default_instance_;
}  // namespace Protocol
PROTOBUF_NAMESPACE_OPEN
template<> ::Protocol::S_CreateLocalGamePlayer* Arena::CreateMaybeMessage<::Protocol::S_CreateLocalGamePlayer>(Arena*);
template<> ::Protocol::S_EnterGame* Arena::CreateMaybeMessage<::Protocol::S_EnterGame>(Arena*);
template<> ::Protocol::S_SyncGameEntities* Arena::CreateMaybeMessage<::Protocol::S_SyncGameEntities>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace Protocol {

// ===================================================================

class S_EnterGame final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:Protocol.S_EnterGame) */ {
 public:
  inline S_EnterGame() : S_EnterGame(nullptr) {}
  ~S_EnterGame() override;
  explicit PROTOBUF_CONSTEXPR S_EnterGame(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  S_EnterGame(const S_EnterGame& from);
  S_EnterGame(S_EnterGame&& from) noexcept
    : S_EnterGame() {
    *this = ::std::move(from);
  }

  inline S_EnterGame& operator=(const S_EnterGame& from) {
    CopyFrom(from);
    return *this;
  }
  inline S_EnterGame& operator=(S_EnterGame&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const S_EnterGame& default_instance() {
    return *internal_default_instance();
  }
  static inline const S_EnterGame* internal_default_instance() {
    return reinterpret_cast<const S_EnterGame*>(
               &_S_EnterGame_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(S_EnterGame& a, S_EnterGame& b) {
    a.Swap(&b);
  }
  inline void Swap(S_EnterGame* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(S_EnterGame* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  S_EnterGame* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<S_EnterGame>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const S_EnterGame& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const S_EnterGame& from) {
    S_EnterGame::MergeImpl(*this, from);
  }
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _impl_._cached_size_.Get(); }

  private:
  void SharedCtor(::PROTOBUF_NAMESPACE_ID::Arena* arena, bool is_message_owned);
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(S_EnterGame* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "Protocol.S_EnterGame";
  }
  protected:
  explicit S_EnterGame(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kUserIdFieldNumber = 2,
    kEnterSuccessFieldNumber = 1,
  };
  // uint64 user_id = 2;
  void clear_user_id();
  uint64_t user_id() const;
  void set_user_id(uint64_t value);
  private:
  uint64_t _internal_user_id() const;
  void _internal_set_user_id(uint64_t value);
  public:

  // bool enter_success = 1;
  void clear_enter_success();
  bool enter_success() const;
  void set_enter_success(bool value);
  private:
  bool _internal_enter_success() const;
  void _internal_set_enter_success(bool value);
  public:

  // @@protoc_insertion_point(class_scope:Protocol.S_EnterGame)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    uint64_t user_id_;
    bool enter_success_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_Packet_2eproto;
};
// -------------------------------------------------------------------

class S_CreateLocalGamePlayer final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:Protocol.S_CreateLocalGamePlayer) */ {
 public:
  inline S_CreateLocalGamePlayer() : S_CreateLocalGamePlayer(nullptr) {}
  ~S_CreateLocalGamePlayer() override;
  explicit PROTOBUF_CONSTEXPR S_CreateLocalGamePlayer(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  S_CreateLocalGamePlayer(const S_CreateLocalGamePlayer& from);
  S_CreateLocalGamePlayer(S_CreateLocalGamePlayer&& from) noexcept
    : S_CreateLocalGamePlayer() {
    *this = ::std::move(from);
  }

  inline S_CreateLocalGamePlayer& operator=(const S_CreateLocalGamePlayer& from) {
    CopyFrom(from);
    return *this;
  }
  inline S_CreateLocalGamePlayer& operator=(S_CreateLocalGamePlayer&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const S_CreateLocalGamePlayer& default_instance() {
    return *internal_default_instance();
  }
  static inline const S_CreateLocalGamePlayer* internal_default_instance() {
    return reinterpret_cast<const S_CreateLocalGamePlayer*>(
               &_S_CreateLocalGamePlayer_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(S_CreateLocalGamePlayer& a, S_CreateLocalGamePlayer& b) {
    a.Swap(&b);
  }
  inline void Swap(S_CreateLocalGamePlayer* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(S_CreateLocalGamePlayer* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  S_CreateLocalGamePlayer* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<S_CreateLocalGamePlayer>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const S_CreateLocalGamePlayer& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const S_CreateLocalGamePlayer& from) {
    S_CreateLocalGamePlayer::MergeImpl(*this, from);
  }
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _impl_._cached_size_.Get(); }

  private:
  void SharedCtor(::PROTOBUF_NAMESPACE_ID::Arena* arena, bool is_message_owned);
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(S_CreateLocalGamePlayer* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "Protocol.S_CreateLocalGamePlayer";
  }
  protected:
  explicit S_CreateLocalGamePlayer(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kLocalGamePlayerInfoFieldNumber = 1,
  };
  // .Protocol.GameEntityInfo local_game_player_info = 1;
  bool has_local_game_player_info() const;
  private:
  bool _internal_has_local_game_player_info() const;
  public:
  void clear_local_game_player_info();
  const ::Protocol::GameEntityInfo& local_game_player_info() const;
  PROTOBUF_NODISCARD ::Protocol::GameEntityInfo* release_local_game_player_info();
  ::Protocol::GameEntityInfo* mutable_local_game_player_info();
  void set_allocated_local_game_player_info(::Protocol::GameEntityInfo* local_game_player_info);
  private:
  const ::Protocol::GameEntityInfo& _internal_local_game_player_info() const;
  ::Protocol::GameEntityInfo* _internal_mutable_local_game_player_info();
  public:
  void unsafe_arena_set_allocated_local_game_player_info(
      ::Protocol::GameEntityInfo* local_game_player_info);
  ::Protocol::GameEntityInfo* unsafe_arena_release_local_game_player_info();

  // @@protoc_insertion_point(class_scope:Protocol.S_CreateLocalGamePlayer)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::Protocol::GameEntityInfo* local_game_player_info_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_Packet_2eproto;
};
// -------------------------------------------------------------------

class S_SyncGameEntities final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:Protocol.S_SyncGameEntities) */ {
 public:
  inline S_SyncGameEntities() : S_SyncGameEntities(nullptr) {}
  ~S_SyncGameEntities() override;
  explicit PROTOBUF_CONSTEXPR S_SyncGameEntities(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  S_SyncGameEntities(const S_SyncGameEntities& from);
  S_SyncGameEntities(S_SyncGameEntities&& from) noexcept
    : S_SyncGameEntities() {
    *this = ::std::move(from);
  }

  inline S_SyncGameEntities& operator=(const S_SyncGameEntities& from) {
    CopyFrom(from);
    return *this;
  }
  inline S_SyncGameEntities& operator=(S_SyncGameEntities&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const S_SyncGameEntities& default_instance() {
    return *internal_default_instance();
  }
  static inline const S_SyncGameEntities* internal_default_instance() {
    return reinterpret_cast<const S_SyncGameEntities*>(
               &_S_SyncGameEntities_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    2;

  friend void swap(S_SyncGameEntities& a, S_SyncGameEntities& b) {
    a.Swap(&b);
  }
  inline void Swap(S_SyncGameEntities* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(S_SyncGameEntities* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  S_SyncGameEntities* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<S_SyncGameEntities>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const S_SyncGameEntities& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const S_SyncGameEntities& from) {
    S_SyncGameEntities::MergeImpl(*this, from);
  }
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _impl_._cached_size_.Get(); }

  private:
  void SharedCtor(::PROTOBUF_NAMESPACE_ID::Arena* arena, bool is_message_owned);
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(S_SyncGameEntities* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "Protocol.S_SyncGameEntities";
  }
  protected:
  explicit S_SyncGameEntities(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kGameEntitiesFieldNumber = 1,
  };
  // repeated .Protocol.GameEntityInfo game_entities = 1;
  int game_entities_size() const;
  private:
  int _internal_game_entities_size() const;
  public:
  void clear_game_entities();
  ::Protocol::GameEntityInfo* mutable_game_entities(int index);
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::Protocol::GameEntityInfo >*
      mutable_game_entities();
  private:
  const ::Protocol::GameEntityInfo& _internal_game_entities(int index) const;
  ::Protocol::GameEntityInfo* _internal_add_game_entities();
  public:
  const ::Protocol::GameEntityInfo& game_entities(int index) const;
  ::Protocol::GameEntityInfo* add_game_entities();
  const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::Protocol::GameEntityInfo >&
      game_entities() const;

  // @@protoc_insertion_point(class_scope:Protocol.S_SyncGameEntities)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::Protocol::GameEntityInfo > game_entities_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_Packet_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// S_EnterGame

// bool enter_success = 1;
inline void S_EnterGame::clear_enter_success() {
  _impl_.enter_success_ = false;
}
inline bool S_EnterGame::_internal_enter_success() const {
  return _impl_.enter_success_;
}
inline bool S_EnterGame::enter_success() const {
  // @@protoc_insertion_point(field_get:Protocol.S_EnterGame.enter_success)
  return _internal_enter_success();
}
inline void S_EnterGame::_internal_set_enter_success(bool value) {
  
  _impl_.enter_success_ = value;
}
inline void S_EnterGame::set_enter_success(bool value) {
  _internal_set_enter_success(value);
  // @@protoc_insertion_point(field_set:Protocol.S_EnterGame.enter_success)
}

// uint64 user_id = 2;
inline void S_EnterGame::clear_user_id() {
  _impl_.user_id_ = uint64_t{0u};
}
inline uint64_t S_EnterGame::_internal_user_id() const {
  return _impl_.user_id_;
}
inline uint64_t S_EnterGame::user_id() const {
  // @@protoc_insertion_point(field_get:Protocol.S_EnterGame.user_id)
  return _internal_user_id();
}
inline void S_EnterGame::_internal_set_user_id(uint64_t value) {
  
  _impl_.user_id_ = value;
}
inline void S_EnterGame::set_user_id(uint64_t value) {
  _internal_set_user_id(value);
  // @@protoc_insertion_point(field_set:Protocol.S_EnterGame.user_id)
}

// -------------------------------------------------------------------

// S_CreateLocalGamePlayer

// .Protocol.GameEntityInfo local_game_player_info = 1;
inline bool S_CreateLocalGamePlayer::_internal_has_local_game_player_info() const {
  return this != internal_default_instance() && _impl_.local_game_player_info_ != nullptr;
}
inline bool S_CreateLocalGamePlayer::has_local_game_player_info() const {
  return _internal_has_local_game_player_info();
}
inline const ::Protocol::GameEntityInfo& S_CreateLocalGamePlayer::_internal_local_game_player_info() const {
  const ::Protocol::GameEntityInfo* p = _impl_.local_game_player_info_;
  return p != nullptr ? *p : reinterpret_cast<const ::Protocol::GameEntityInfo&>(
      ::Protocol::_GameEntityInfo_default_instance_);
}
inline const ::Protocol::GameEntityInfo& S_CreateLocalGamePlayer::local_game_player_info() const {
  // @@protoc_insertion_point(field_get:Protocol.S_CreateLocalGamePlayer.local_game_player_info)
  return _internal_local_game_player_info();
}
inline void S_CreateLocalGamePlayer::unsafe_arena_set_allocated_local_game_player_info(
    ::Protocol::GameEntityInfo* local_game_player_info) {
  if (GetArenaForAllocation() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(_impl_.local_game_player_info_);
  }
  _impl_.local_game_player_info_ = local_game_player_info;
  if (local_game_player_info) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:Protocol.S_CreateLocalGamePlayer.local_game_player_info)
}
inline ::Protocol::GameEntityInfo* S_CreateLocalGamePlayer::release_local_game_player_info() {
  
  ::Protocol::GameEntityInfo* temp = _impl_.local_game_player_info_;
  _impl_.local_game_player_info_ = nullptr;
#ifdef PROTOBUF_FORCE_COPY_IN_RELEASE
  auto* old =  reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(temp);
  temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  if (GetArenaForAllocation() == nullptr) { delete old; }
#else  // PROTOBUF_FORCE_COPY_IN_RELEASE
  if (GetArenaForAllocation() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
#endif  // !PROTOBUF_FORCE_COPY_IN_RELEASE
  return temp;
}
inline ::Protocol::GameEntityInfo* S_CreateLocalGamePlayer::unsafe_arena_release_local_game_player_info() {
  // @@protoc_insertion_point(field_release:Protocol.S_CreateLocalGamePlayer.local_game_player_info)
  
  ::Protocol::GameEntityInfo* temp = _impl_.local_game_player_info_;
  _impl_.local_game_player_info_ = nullptr;
  return temp;
}
inline ::Protocol::GameEntityInfo* S_CreateLocalGamePlayer::_internal_mutable_local_game_player_info() {
  
  if (_impl_.local_game_player_info_ == nullptr) {
    auto* p = CreateMaybeMessage<::Protocol::GameEntityInfo>(GetArenaForAllocation());
    _impl_.local_game_player_info_ = p;
  }
  return _impl_.local_game_player_info_;
}
inline ::Protocol::GameEntityInfo* S_CreateLocalGamePlayer::mutable_local_game_player_info() {
  ::Protocol::GameEntityInfo* _msg = _internal_mutable_local_game_player_info();
  // @@protoc_insertion_point(field_mutable:Protocol.S_CreateLocalGamePlayer.local_game_player_info)
  return _msg;
}
inline void S_CreateLocalGamePlayer::set_allocated_local_game_player_info(::Protocol::GameEntityInfo* local_game_player_info) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArenaForAllocation();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(_impl_.local_game_player_info_);
  }
  if (local_game_player_info) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
        ::PROTOBUF_NAMESPACE_ID::Arena::InternalGetOwningArena(
                reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(local_game_player_info));
    if (message_arena != submessage_arena) {
      local_game_player_info = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, local_game_player_info, submessage_arena);
    }
    
  } else {
    
  }
  _impl_.local_game_player_info_ = local_game_player_info;
  // @@protoc_insertion_point(field_set_allocated:Protocol.S_CreateLocalGamePlayer.local_game_player_info)
}

// -------------------------------------------------------------------

// S_SyncGameEntities

// repeated .Protocol.GameEntityInfo game_entities = 1;
inline int S_SyncGameEntities::_internal_game_entities_size() const {
  return _impl_.game_entities_.size();
}
inline int S_SyncGameEntities::game_entities_size() const {
  return _internal_game_entities_size();
}
inline ::Protocol::GameEntityInfo* S_SyncGameEntities::mutable_game_entities(int index) {
  // @@protoc_insertion_point(field_mutable:Protocol.S_SyncGameEntities.game_entities)
  return _impl_.game_entities_.Mutable(index);
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::Protocol::GameEntityInfo >*
S_SyncGameEntities::mutable_game_entities() {
  // @@protoc_insertion_point(field_mutable_list:Protocol.S_SyncGameEntities.game_entities)
  return &_impl_.game_entities_;
}
inline const ::Protocol::GameEntityInfo& S_SyncGameEntities::_internal_game_entities(int index) const {
  return _impl_.game_entities_.Get(index);
}
inline const ::Protocol::GameEntityInfo& S_SyncGameEntities::game_entities(int index) const {
  // @@protoc_insertion_point(field_get:Protocol.S_SyncGameEntities.game_entities)
  return _internal_game_entities(index);
}
inline ::Protocol::GameEntityInfo* S_SyncGameEntities::_internal_add_game_entities() {
  return _impl_.game_entities_.Add();
}
inline ::Protocol::GameEntityInfo* S_SyncGameEntities::add_game_entities() {
  ::Protocol::GameEntityInfo* _add = _internal_add_game_entities();
  // @@protoc_insertion_point(field_add:Protocol.S_SyncGameEntities.game_entities)
  return _add;
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::Protocol::GameEntityInfo >&
S_SyncGameEntities::game_entities() const {
  // @@protoc_insertion_point(field_list:Protocol.S_SyncGameEntities.game_entities)
  return _impl_.game_entities_;
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace Protocol

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_Packet_2eproto
