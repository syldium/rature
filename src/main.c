#include <stdio.h>
#include <stdlib.h>

#include <mupdf/fitz.h>
#include <mupdf/pdf.h>

#include "redact.h"

int main(int argc, char *argv[])
{
    if (argc < 4) {
        fprintf(stderr, "Usage: %s input-file redacted-text output-file\n", argv[0]);
        return EXIT_FAILURE;
    }

    char* input = argv[1];
    char* redacted = argv[2];
    char* output = argv[3];

    fz_context* ctx;
    pdf_document* doc;

    /* Create a context to hold the exception stack and various caches. */
    ctx = fz_new_context(NULL, NULL, FZ_STORE_DEFAULT);
    if (!ctx) {
        fprintf(stderr, "cannot create mupdf context\n");
        return EXIT_FAILURE;
    }

    /* Open the document. */
    fz_try(ctx) {
        doc = pdf_open_document(ctx, input);
    } fz_catch(ctx) {
        fz_report_error(ctx);
        fz_drop_context(ctx);
        return EXIT_FAILURE;
    }

    /* Loop through each page in the document. */
    int page_count = pdf_count_pages(ctx, doc);
    for (int i = 0; i < page_count; i++) {
        pdf_page* page;

        /* Load the page. */
        fz_try(ctx) {
            page = pdf_load_page(ctx, doc, i);
        } fz_catch(ctx) {
            fz_report_error(ctx);
            break;
        }

        if (redact_page(ctx, page, redacted) != 0) {
            pdf_drop_page(ctx, page);
            break;
        }
        pdf_drop_page(ctx, page);
    }

    /* Save the document. */
    fz_try(ctx) {
        pdf_save_document(ctx, doc, output, NULL);
    } fz_catch(ctx) {
        fz_report_error(ctx);
        fprintf(stderr, "cannot save document\n");
        pdf_drop_document(ctx, doc);
        fz_drop_context(ctx);
        return EXIT_FAILURE;
    }

    /* Clean up. */
    pdf_drop_document(ctx, doc);
    fz_drop_context(ctx);
    return EXIT_SUCCESS;
}
