#ifndef RATURE_REDACT_H
#define RATURE_REDACT_H

#include <mupdf/fitz.h>
#include <mupdf/pdf.h>

int redact_page(fz_context *ctx, pdf_page *page, const char *text);

#endif // RATURE_REDACT_H
