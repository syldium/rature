#include "redact.h"

static int redact_cb(fz_context *ctx, void *opaque, int num_quads, fz_quad *hit_bbox) {
    pdf_page *page = opaque;
    for (int i = 0; i < num_quads; i++) {
        fz_quad quad = hit_bbox[i];
        fz_rect rect = fz_rect_from_quad(quad);

        pdf_annot *annot;
        fz_try(ctx) {
            annot = pdf_create_annot(ctx, page, PDF_ANNOT_REDACT);
        } fz_catch(ctx) {
            fz_warn(ctx, "cannot create redact annotation");
            return 1;
        }
        pdf_set_annot_rect(ctx, annot, rect);
        pdf_update_annot(ctx, annot);
        pdf_drop_annot(ctx, annot);
    }
    return 0;
}

int redact_page(fz_context *ctx, pdf_page *page, const char *text) {
    pdf_redact_options redact_opts = {
        .black_boxes = 1,
        .image_method = PDF_REDACT_IMAGE_PIXELS,
    };

    fz_search_page_cb(ctx, &page->super, text, redact_cb, page);
    fz_try(ctx) {
        pdf_redact_page(ctx, page->doc, page, &redact_opts);
    } fz_catch(ctx) {
        fz_report_error(ctx);
        return 1;
    }
    return 0;
}
