#include "export.h"

#include "include/core/SkStream.h"


extern "C" {

QUEST_API SkDynamicMemoryWStream *write_stream_create() {
    return new SkDynamicMemoryWStream();
}

QUEST_API void write_stream_delete(SkDynamicMemoryWStream *stream) {
    delete stream;
}

QUEST_API SkData *write_stream_detach_data(SkDynamicMemoryWStream *stream) {
    return stream->detachAsData().release();
}

}