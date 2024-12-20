#include "mime_type.h"

#include <string.h>

// requester
#include <utils/utils.h>

#include "mime_type_text.h"

typedef struct {
    const char* extension;
    MIMEType mime_type;
} MIMEMap;

static const MIMEMap mime_map[] = {
    {"json", MIME_APPLICATION_JSON},
    {"xml", MIME_APPLICATION_XML},
    {"urlencoded", MIME_APPLICATION_X_WWW_FORM_URLENCODED},
    {"txt", MIME_TEXT_PLAIN},
    {"html", MIME_TEXT_HTML},
    {"htm", MIME_TEXT_HTML},
    {"css", MIME_TEXT_CSS},
    {"js", MIME_TEXT_JAVASCRIPT},
    {"png", MIME_IMAGE_PNG},
    {"jpeg", MIME_IMAGE_JPEG},
    {"jpg", MIME_IMAGE_JPEG},
    {"webp", MIME_IMAGE_WEBP},
    {"gif", MIME_IMAGE_GIF},
    {"bmp", MIME_IMAGE_BMP},
    {"mp3", MIME_AUDIO_MPEG},
    {"ogg", MIME_AUDIO_OGG},
    {"wav", MIME_AUDIO_WAV},
    {"flac", MIME_AUDIO_FLAC},
    {"mp4", MIME_VIDEO_MP4},
    {"avi", MIME_VIDEO_AVI},
    {"webm", MIME_VIDEO_WEBM},
    {"pdf", MIME_APPLICATION_PDF},
    {"zip", MIME_APPLICATION_ZIP},
    {"bin", MIME_APPLICATION_OCTET_STREAM},
    {"tar", MIME_APPLICATION_X_TAR},
    {"gz", MIME_APPLICATION_GZIP},
    {"woff", MIME_FONT_WOFF},
    {"woff2", MIME_FONT_WOFF2},
    {"ttf", MIME_FONT_TTF},
    {"eot", MIME_FONT_EOT},
    {"rss", MIME_APPLICATION_RSS_XML},
    {"rar", MIME_APPLICATION_X_RAR_COMPRESSION},
    {"xls", MIME_APPLICATION_VND_MS_EXCEL},
    {"ppt", MIME_APPLICATION_VND_MS_POWERPOINT},
    {"doc", MIME_APPLICATION_VND_MS_WORD},
    {"jsonld", MIME_APPLICATION_JSONLD},
};

MIMEType MIMEType_FromFileName(const char* file_name) {
    if (file_name == NULL) {
        return MIME_UNKNOWN;
    }

    /*
        Find the extension position.
    */
    const char* ext_start = NULL;
    const char* period = NULL;

    do {
        ext_start = period;

        period = strchr(file_name, '.');

    } while (period != NULL);

    if (ext_start == NULL) {
        return MIME_UNKNOWN;
    }

    ext_start++;

    /*
        If found, start comparing each well-known extension.
    */
    size_t i;

    for (i = 0; i < ARRAY_SIZE(mime_map); ++i) {
        if (strcmp(ext_start, mime_map[i].extension) == 0) {
            return mime_map[i].mime_type;
        }
    }

    return MIME_UNKNOWN;
}

#define MIME_TYPE_CHECK(target) \
    if (strncmp(target##_TEXT, src, src_size) == 0) return target

MIMEType MIMEType_FromString(const char* src, const size_t src_size) {
    if (src == NULL || src_size == 0) {
        return MIME_UNKNOWN;
    }

    MIME_TYPE_CHECK(MIME_APPLICATION_JSON);
    MIME_TYPE_CHECK(MIME_APPLICATION_XML);
    MIME_TYPE_CHECK(MIME_APPLICATION_X_WWW_FORM_URLENCODED);
    MIME_TYPE_CHECK(MIME_MULTIPART_FORM_DATA);
    MIME_TYPE_CHECK(MIME_TEXT_PLAIN);
    MIME_TYPE_CHECK(MIME_TEXT_HTML);
    MIME_TYPE_CHECK(MIME_TEXT_CSS);
    MIME_TYPE_CHECK(MIME_TEXT_JAVASCRIPT);
    MIME_TYPE_CHECK(MIME_IMAGE_PNG);
    MIME_TYPE_CHECK(MIME_IMAGE_JPEG);
    MIME_TYPE_CHECK(MIME_IMAGE_WEBP);
    MIME_TYPE_CHECK(MIME_IMAGE_GIF);
    MIME_TYPE_CHECK(MIME_IMAGE_BMP);
    MIME_TYPE_CHECK(MIME_AUDIO_MPEG);
    MIME_TYPE_CHECK(MIME_AUDIO_OGG);
    MIME_TYPE_CHECK(MIME_AUDIO_WAV);
    MIME_TYPE_CHECK(MIME_AUDIO_FLAC);
    MIME_TYPE_CHECK(MIME_VIDEO_MP4);
    MIME_TYPE_CHECK(MIME_VIDEO_AVI);
    MIME_TYPE_CHECK(MIME_VIDEO_WEBM);
    MIME_TYPE_CHECK(MIME_APPLICATION_PDF);
    MIME_TYPE_CHECK(MIME_APPLICATION_ZIP);
    MIME_TYPE_CHECK(MIME_APPLICATION_OCTET_STREAM);
    MIME_TYPE_CHECK(MIME_APPLICATION_X_TAR);
    MIME_TYPE_CHECK(MIME_APPLICATION_GZIP);
    MIME_TYPE_CHECK(MIME_FONT_WOFF);
    MIME_TYPE_CHECK(MIME_FONT_WOFF2);
    MIME_TYPE_CHECK(MIME_FONT_TTF);
    MIME_TYPE_CHECK(MIME_FONT_EOT);
    MIME_TYPE_CHECK(MIME_APPLICATION_RSS_XML);
    MIME_TYPE_CHECK(MIME_APPLICATION_X_RAR_COMPRESSION);
    MIME_TYPE_CHECK(MIME_APPLICATION_JS);
    MIME_TYPE_CHECK(MIME_APPLICATION_VND_MS_EXCEL);
    MIME_TYPE_CHECK(MIME_APPLICATION_VND_MS_POWERPOINT);
    MIME_TYPE_CHECK(MIME_APPLICATION_VND_MS_WORD);
    MIME_TYPE_CHECK(MIME_APPLICATION_JSONLD);

    return MIME_UNKNOWN;
}

const char* MIMEType_ToString(MIMEType mt) {
    switch (mt) {
        case MIME_UNKNOWN:
            return MIME_UNKNOWN_TEXT;
        case MIME_APPLICATION_JSON:
            return MIME_APPLICATION_JSON_TEXT;
        case MIME_APPLICATION_XML:
            return MIME_APPLICATION_XML_TEXT;
        case MIME_APPLICATION_X_WWW_FORM_URLENCODED:
            return MIME_APPLICATION_X_WWW_FORM_URLENCODED_TEXT;
        case MIME_MULTIPART_FORM_DATA:
            return MIME_MULTIPART_FORM_DATA_TEXT;
        case MIME_TEXT_PLAIN:
            return MIME_TEXT_PLAIN_TEXT;
        case MIME_TEXT_HTML:
            return MIME_TEXT_HTML_TEXT;
        case MIME_TEXT_CSS:
            return MIME_TEXT_CSS_TEXT;
        case MIME_TEXT_JAVASCRIPT:
            return MIME_TEXT_JAVASCRIPT_TEXT;
        case MIME_IMAGE_PNG:
            return MIME_IMAGE_PNG_TEXT;
        case MIME_IMAGE_JPEG:
            return MIME_IMAGE_JPEG_TEXT;
        case MIME_IMAGE_WEBP:
            return MIME_IMAGE_WEBP_TEXT;
        case MIME_IMAGE_GIF:
            return MIME_IMAGE_GIF_TEXT;
        case MIME_IMAGE_BMP:
            return MIME_IMAGE_BMP_TEXT;
        case MIME_AUDIO_MPEG:
            return MIME_AUDIO_MPEG_TEXT;
        case MIME_AUDIO_OGG:
            return MIME_AUDIO_OGG_TEXT;
        case MIME_AUDIO_WAV:
            return MIME_AUDIO_WAV_TEXT;
        case MIME_AUDIO_FLAC:
            return MIME_AUDIO_FLAC_TEXT;
        case MIME_VIDEO_MP4:
            return MIME_VIDEO_MP4_TEXT;
        case MIME_VIDEO_AVI:
            return MIME_VIDEO_AVI_TEXT;
        case MIME_VIDEO_WEBM:
            return MIME_VIDEO_WEBM_TEXT;
        case MIME_APPLICATION_PDF:
            return MIME_APPLICATION_PDF_TEXT;
        case MIME_APPLICATION_ZIP:
            return MIME_APPLICATION_ZIP_TEXT;
        case MIME_APPLICATION_OCTET_STREAM:
            return MIME_APPLICATION_OCTET_STREAM_TEXT;
        case MIME_APPLICATION_X_TAR:
            return MIME_APPLICATION_X_TAR_TEXT;
        case MIME_APPLICATION_GZIP:
            return MIME_APPLICATION_GZIP_TEXT;
        case MIME_FONT_WOFF:
            return MIME_FONT_WOFF_TEXT;
        case MIME_FONT_WOFF2:
            return MIME_FONT_WOFF2_TEXT;
        case MIME_FONT_TTF:
            return MIME_FONT_TTF_TEXT;
        case MIME_FONT_EOT:
            return MIME_FONT_EOT_TEXT;
        case MIME_APPLICATION_RSS_XML:
            return MIME_APPLICATION_RSS_XML_TEXT;
        case MIME_APPLICATION_X_RAR_COMPRESSION:
            return MIME_APPLICATION_X_RAR_COMPRESSION_TEXT;
        case MIME_APPLICATION_JS:
            return MIME_APPLICATION_JS_TEXT;
        case MIME_APPLICATION_VND_MS_EXCEL:
            return MIME_APPLICATION_VND_MS_EXCEL_TEXT;
        case MIME_APPLICATION_VND_MS_POWERPOINT:
            return MIME_APPLICATION_VND_MS_POWERPOINT_TEXT;
        case MIME_APPLICATION_VND_MS_WORD:
            return MIME_APPLICATION_VND_MS_WORD_TEXT;
        case MIME_APPLICATION_JSONLD:
            return MIME_APPLICATION_JSONLD_TEXT;
    }

    return MIME_UNKNOWN_TEXT;
}
