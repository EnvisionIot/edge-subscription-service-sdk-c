/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: setmeasurepointresponsepoint.proto */

/* Do not generate deprecated warnings for self */
#ifndef PROTOBUF_C__NO_DEPRECATED
#define PROTOBUF_C__NO_DEPRECATED
#endif

#include "setmeasurepointresponsepoint.pb-c.h"
void   setmeasurepointresponsedata__set_measurepoint_response_point__init
                     (Setmeasurepointresponsedata__SetMeasurepointResponsePoint         *message)
{
  static const Setmeasurepointresponsedata__SetMeasurepointResponsePoint init_value = SETMEASUREPOINTRESPONSEDATA__SET_MEASUREPOINT_RESPONSE_POINT__INIT;
  *message = init_value;
}
size_t setmeasurepointresponsedata__set_measurepoint_response_point__get_packed_size
                     (const Setmeasurepointresponsedata__SetMeasurepointResponsePoint *message)
{
  assert(message->base.descriptor == &setmeasurepointresponsedata__set_measurepoint_response_point__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t setmeasurepointresponsedata__set_measurepoint_response_point__pack
                     (const Setmeasurepointresponsedata__SetMeasurepointResponsePoint *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &setmeasurepointresponsedata__set_measurepoint_response_point__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t setmeasurepointresponsedata__set_measurepoint_response_point__pack_to_buffer
                     (const Setmeasurepointresponsedata__SetMeasurepointResponsePoint *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &setmeasurepointresponsedata__set_measurepoint_response_point__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Setmeasurepointresponsedata__SetMeasurepointResponsePoint *
       setmeasurepointresponsedata__set_measurepoint_response_point__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Setmeasurepointresponsedata__SetMeasurepointResponsePoint *)
     protobuf_c_message_unpack (&setmeasurepointresponsedata__set_measurepoint_response_point__descriptor,
                                allocator, len, data);
}
void   setmeasurepointresponsedata__set_measurepoint_response_point__free_unpacked
                     (Setmeasurepointresponsedata__SetMeasurepointResponsePoint *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &setmeasurepointresponsedata__set_measurepoint_response_point__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   setmeasurepointresponsedata__set_measurepoint_response_points__init
                     (Setmeasurepointresponsedata__SetMeasurepointResponsePoints         *message)
{
  static const Setmeasurepointresponsedata__SetMeasurepointResponsePoints init_value = SETMEASUREPOINTRESPONSEDATA__SET_MEASUREPOINT_RESPONSE_POINTS__INIT;
  *message = init_value;
}
size_t setmeasurepointresponsedata__set_measurepoint_response_points__get_packed_size
                     (const Setmeasurepointresponsedata__SetMeasurepointResponsePoints *message)
{
  assert(message->base.descriptor == &setmeasurepointresponsedata__set_measurepoint_response_points__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t setmeasurepointresponsedata__set_measurepoint_response_points__pack
                     (const Setmeasurepointresponsedata__SetMeasurepointResponsePoints *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &setmeasurepointresponsedata__set_measurepoint_response_points__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t setmeasurepointresponsedata__set_measurepoint_response_points__pack_to_buffer
                     (const Setmeasurepointresponsedata__SetMeasurepointResponsePoints *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &setmeasurepointresponsedata__set_measurepoint_response_points__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Setmeasurepointresponsedata__SetMeasurepointResponsePoints *
       setmeasurepointresponsedata__set_measurepoint_response_points__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Setmeasurepointresponsedata__SetMeasurepointResponsePoints *)
     protobuf_c_message_unpack (&setmeasurepointresponsedata__set_measurepoint_response_points__descriptor,
                                allocator, len, data);
}
void   setmeasurepointresponsedata__set_measurepoint_response_points__free_unpacked
                     (Setmeasurepointresponsedata__SetMeasurepointResponsePoints *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &setmeasurepointresponsedata__set_measurepoint_response_points__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
static const ProtobufCFieldDescriptor setmeasurepointresponsedata__set_measurepoint_response_point__field_descriptors[17] =
{
  {
    "requestId",
    1,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Setmeasurepointresponsedata__SetMeasurepointResponsePoint, requestid),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "orgId",
    2,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Setmeasurepointresponsedata__SetMeasurepointResponsePoint, orgid),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "callType",
    3,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Setmeasurepointresponsedata__SetMeasurepointResponsePoint, calltype),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "setMeasurepointChannelId",
    4,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Setmeasurepointresponsedata__SetMeasurepointResponsePoint, setmeasurepointchannelid),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "productKey",
    5,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Setmeasurepointresponsedata__SetMeasurepointResponsePoint, productkey),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "deviceKey",
    6,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Setmeasurepointresponsedata__SetMeasurepointResponsePoint, devicekey),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "assetId",
    7,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Setmeasurepointresponsedata__SetMeasurepointResponsePoint, assetid),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "measurepointId",
    8,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Setmeasurepointresponsedata__SetMeasurepointResponsePoint, measurepointid),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "callbackUrl",
    9,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Setmeasurepointresponsedata__SetMeasurepointResponsePoint, callbackurl),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "inputData",
    10,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Setmeasurepointresponsedata__SetMeasurepointResponsePoint, inputdata),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "status",
    11,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_INT64,
    0,   /* quantifier_offset */
    offsetof(Setmeasurepointresponsedata__SetMeasurepointResponsePoint, status),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "msg",
    12,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Setmeasurepointresponsedata__SetMeasurepointResponsePoint, msg),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "submsg",
    13,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Setmeasurepointresponsedata__SetMeasurepointResponsePoint, submsg),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "timeout",
    14,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_INT64,
    0,   /* quantifier_offset */
    offsetof(Setmeasurepointresponsedata__SetMeasurepointResponsePoint, timeout),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "gmtSetMeasurepointRequest",
    15,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_INT64,
    0,   /* quantifier_offset */
    offsetof(Setmeasurepointresponsedata__SetMeasurepointResponsePoint, gmtsetmeasurepointrequest),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "gmtSetMeasurepointReply",
    16,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_INT64,
    0,   /* quantifier_offset */
    offsetof(Setmeasurepointresponsedata__SetMeasurepointResponsePoint, gmtsetmeasurepointreply),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "attr",
    17,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Setmeasurepointresponsedata__SetMeasurepointResponsePoint, attr),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned setmeasurepointresponsedata__set_measurepoint_response_point__field_indices_by_name[] = {
  6,   /* field[6] = assetId */
  16,   /* field[16] = attr */
  2,   /* field[2] = callType */
  8,   /* field[8] = callbackUrl */
  5,   /* field[5] = deviceKey */
  15,   /* field[15] = gmtSetMeasurepointReply */
  14,   /* field[14] = gmtSetMeasurepointRequest */
  9,   /* field[9] = inputData */
  7,   /* field[7] = measurepointId */
  11,   /* field[11] = msg */
  1,   /* field[1] = orgId */
  4,   /* field[4] = productKey */
  0,   /* field[0] = requestId */
  3,   /* field[3] = setMeasurepointChannelId */
  10,   /* field[10] = status */
  12,   /* field[12] = submsg */
  13,   /* field[13] = timeout */
};
static const ProtobufCIntRange setmeasurepointresponsedata__set_measurepoint_response_point__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 17 }
};
const ProtobufCMessageDescriptor setmeasurepointresponsedata__set_measurepoint_response_point__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "setmeasurepointresponsedata.SetMeasurepointResponsePoint",
  "SetMeasurepointResponsePoint",
  "Setmeasurepointresponsedata__SetMeasurepointResponsePoint",
  "setmeasurepointresponsedata",
  sizeof(Setmeasurepointresponsedata__SetMeasurepointResponsePoint),
  17,
  setmeasurepointresponsedata__set_measurepoint_response_point__field_descriptors,
  setmeasurepointresponsedata__set_measurepoint_response_point__field_indices_by_name,
  1,  setmeasurepointresponsedata__set_measurepoint_response_point__number_ranges,
  (ProtobufCMessageInit) setmeasurepointresponsedata__set_measurepoint_response_point__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor setmeasurepointresponsedata__set_measurepoint_response_points__field_descriptors[1] =
{
  {
    "points",
    1,
    PROTOBUF_C_LABEL_REPEATED,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(Setmeasurepointresponsedata__SetMeasurepointResponsePoints, n_points),
    offsetof(Setmeasurepointresponsedata__SetMeasurepointResponsePoints, points),
    &setmeasurepointresponsedata__set_measurepoint_response_point__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned setmeasurepointresponsedata__set_measurepoint_response_points__field_indices_by_name[] = {
  0,   /* field[0] = points */
};
static const ProtobufCIntRange setmeasurepointresponsedata__set_measurepoint_response_points__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 1 }
};
const ProtobufCMessageDescriptor setmeasurepointresponsedata__set_measurepoint_response_points__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "setmeasurepointresponsedata.SetMeasurepointResponsePoints",
  "SetMeasurepointResponsePoints",
  "Setmeasurepointresponsedata__SetMeasurepointResponsePoints",
  "setmeasurepointresponsedata",
  sizeof(Setmeasurepointresponsedata__SetMeasurepointResponsePoints),
  1,
  setmeasurepointresponsedata__set_measurepoint_response_points__field_descriptors,
  setmeasurepointresponsedata__set_measurepoint_response_points__field_indices_by_name,
  1,  setmeasurepointresponsedata__set_measurepoint_response_points__number_ranges,
  (ProtobufCMessageInit) setmeasurepointresponsedata__set_measurepoint_response_points__init,
  NULL,NULL,NULL    /* reserved[123] */
};
