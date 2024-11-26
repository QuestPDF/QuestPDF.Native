#include "export.h"

#include "include/core/SkPicture.h"
#include "include/core/SkPictureRecorder.h"

extern "C" {

QUEST_API SkPictureRecorder *picture_recorder_create() {
    return new SkPictureRecorder();
}

QUEST_API SkCanvas *picture_recorder_begin_recording(SkPictureRecorder *pictureRecorder, float width, float height) {
    return pictureRecorder->beginRecording(width, height);
}

QUEST_API SkPicture *picture_recorder_end_recording_with_picture(SkPictureRecorder *pictureRecorder) {
    return pictureRecorder->finishRecordingAsPicture().release();
}

QUEST_API SkDrawable *picture_recorder_end_recording_with_drawable(SkPictureRecorder *pictureRecorder) {
    return pictureRecorder->finishRecordingAsDrawable().release();
}

QUEST_API void picture_recorder_delete(SkPictureRecorder *pictureRecorder) {
    delete pictureRecorder;
}

}