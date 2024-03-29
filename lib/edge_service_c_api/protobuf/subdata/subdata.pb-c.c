/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: subdata.proto */

/* Do not generate deprecated warnings for self */
#ifndef PROTOBUF_C__NO_DEPRECATED
#define PROTOBUF_C__NO_DEPRECATED
#endif

#include "subdata.pb-c.h"
void   subdata__sub_point__init
                     (Subdata__SubPoint         *message)
{
  static const Subdata__SubPoint init_value = SUBDATA__SUB_POINT__INIT;
  *message = init_value;
}
size_t subdata__sub_point__get_packed_size
                     (const Subdata__SubPoint *message)
{
  assert(message->base.descriptor == &subdata__sub_point__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t subdata__sub_point__pack
                     (const Subdata__SubPoint *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &subdata__sub_point__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t subdata__sub_point__pack_to_buffer
                     (const Subdata__SubPoint *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &subdata__sub_point__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Subdata__SubPoint *
       subdata__sub_point__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Subdata__SubPoint *)
     protobuf_c_message_unpack (&subdata__sub_point__descriptor,
                                allocator, len, data);
}
void   subdata__sub_point__free_unpacked
                     (Subdata__SubPoint *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &subdata__sub_point__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   subdata__sub_points__init
                     (Subdata__SubPoints         *message)
{
  static const Subdata__SubPoints init_value = SUBDATA__SUB_POINTS__INIT;
  *message = init_value;
}
size_t subdata__sub_points__get_packed_size
                     (const Subdata__SubPoints *message)
{
  assert(message->base.descriptor == &subdata__sub_points__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t subdata__sub_points__pack
                     (const Subdata__SubPoints *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &subdata__sub_points__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t subdata__sub_points__pack_to_buffer
                     (const Subdata__SubPoints *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &subdata__sub_points__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Subdata__SubPoints *
       subdata__sub_points__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Subdata__SubPoints *)
     protobuf_c_message_unpack (&subdata__sub_points__descriptor,
                                allocator, len, data);
}
void   subdata__sub_points__free_unpacked
                     (Subdata__SubPoints *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &subdata__sub_points__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
static const ProtobufCFieldDescriptor subdata__sub_point__field_descriptors[13] =
{
  {
    "assetId",
    1,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Subdata__SubPoint, assetid),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "pointId",
    2,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Subdata__SubPoint, pointid),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "time",
    3,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_INT64,
    offsetof(Subdata__SubPoint, has_time),
    offsetof(Subdata__SubPoint, time),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "value",
    4,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Subdata__SubPoint, value),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "quality",
    5,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_INT32,
    offsetof(Subdata__SubPoint, has_quality),
    offsetof(Subdata__SubPoint, quality),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "edq",
    6,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_INT32,
    offsetof(Subdata__SubPoint, has_edq),
    offsetof(Subdata__SubPoint, edq),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "dataType",
    7,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Subdata__SubPoint, datatype),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "subDataType",
    8,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Subdata__SubPoint, subdatatype),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "oemTime",
    9,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_INT64,
    offsetof(Subdata__SubPoint, has_oemtime),
    offsetof(Subdata__SubPoint, oemtime),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "attr",
    10,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Subdata__SubPoint, attr),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "spanId",
    11,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Subdata__SubPoint, spanid),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "traceId",
    12,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Subdata__SubPoint, traceid),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "sendremote",
    13,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_BOOL,
    offsetof(Subdata__SubPoint, has_sendremote),
    offsetof(Subdata__SubPoint, sendremote),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned subdata__sub_point__field_indices_by_name[] = {
  0,   /* field[0] = assetId */
  9,   /* field[9] = attr */
  6,   /* field[6] = dataType */
  5,   /* field[5] = edq */
  8,   /* field[8] = oemTime */
  1,   /* field[1] = pointId */
  4,   /* field[4] = quality */
  12,   /* field[12] = sendremote */
  10,   /* field[10] = spanId */
  7,   /* field[7] = subDataType */
  2,   /* field[2] = time */
  11,   /* field[11] = traceId */
  3,   /* field[3] = value */
};
static const ProtobufCIntRange subdata__sub_point__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 13 }
};
const ProtobufCMessageDescriptor subdata__sub_point__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "subdata.SubPoint",
  "SubPoint",
  "Subdata__SubPoint",
  "subdata",
  sizeof(Subdata__SubPoint),
  13,
  subdata__sub_point__field_descriptors,
  subdata__sub_point__field_indices_by_name,
  1,  subdata__sub_point__number_ranges,
  (ProtobufCMessageInit) subdata__sub_point__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor subdata__sub_points__field_descriptors[1] =
{
  {
    "points",
    1,
    PROTOBUF_C_LABEL_REPEATED,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(Subdata__SubPoints, n_points),
    offsetof(Subdata__SubPoints, points),
    &subdata__sub_point__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned subdata__sub_points__field_indices_by_name[] = {
  0,   /* field[0] = points */
};
static const ProtobufCIntRange subdata__sub_points__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 1 }
};
const ProtobufCMessageDescriptor subdata__sub_points__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "subdata.SubPoints",
  "SubPoints",
  "Subdata__SubPoints",
  "subdata",
  sizeof(Subdata__SubPoints),
  1,
  subdata__sub_points__field_descriptors,
  subdata__sub_points__field_indices_by_name,
  1,  subdata__sub_points__number_ranges,
  (ProtobufCMessageInit) subdata__sub_points__init,
  NULL,NULL,NULL    /* reserved[123] */
};
