#TODO: so far not reviewed, only written by ChatGPT

# Improved "handout" filter for beamer PDFs:
# Keep only the LAST page in each sequence of overlays where the next page's text contains
# (or equals) the current page's text. This tends to retain only the final "all bullets visible" slide.
from PyPDF2 import PdfReader, PdfWriter
from difflib import SequenceMatcher

input_path = str(input("> Provide name of beamer pdf to be cleaned: "))
output_path = input_path[:-4] + "_cleaned" + ".pdf"

def normalize_text(s: str) -> str:
    if not s:
        return ""
    # Lowercase, collapse whitespace, strip page numbers/punctuation runs
    base = " ".join(s.split()).lower()
    return base

def text_contains(a: str, b: str) -> bool:
    # True if 'a' is (almost) contained in 'b'.
    if not a and not b:
        return True
    if not a and b:
        return True
    if a == b:
        return True
    if len(a) <= len(b) and a in b:
        return True
    # Fallback: similarity threshold (handles minor reflows)
    sm = SequenceMatcher(None, a, b)
    return sm.quick_ratio() > 0.98  # very strict

reader = PdfReader(input_path)

# Extract normalized text for all pages
texts = []
for p in reader.pages:
    try:
        t = p.extract_text()
    except Exception:
        t = ""
    texts.append(normalize_text(t))

keep_indices = []

i = 0
n = len(reader.pages)
while i < n:
    # advance until the "chain" ends (as long as next contains current)
    j = i
    while j + 1 < n and text_contains(texts[j], texts[j+1]):
        j += 1
    # keep only the LAST page of this chain
    keep_indices.append(j)
    i = j + 1

# Write output
writer = PdfWriter()
for idx in keep_indices:
    writer.add_page(reader.pages[idx])

with open(output_path, "wb") as f:
    writer.write(f)

removed = n - len(keep_indices)
print({"kept_pages": len(keep_indices), "removed_pages": removed, "output": output_path})