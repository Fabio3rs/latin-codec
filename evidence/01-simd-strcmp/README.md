# Experiment 01 — SIMD String Comparison: Latin vs English Prompt

## The Core Experiment

This is the central case study. The same technical content was sent to ChatGPT in two different representations, and the model responded in fundamentally different modes.

## Source Material

A C++ header (`simd_strcmp.h`) implementing AVX2-based string comparison with:
- Two algorithms: `simd_strcmp_chunkenizer` and `simd_strcmp_sub`
- MSVC `_BitScanForward` / `__builtin_ctz` macro
- A custom iterator pattern (`Chunkenizer`) returning `std::tuple`
- `noexcept` on all functions
- AVX2 intrinsics throughout

The original code before refactoring is in `original.cpp`.
The refactored header is in `simd_strcmp.h`.

## The Two Prompts

**Neither prompt contained the source code.** Only the dense summary was provided.

### Latin (Lapidary)
> See [prompt-latin.md](prompt-latin.md)

### English (Google Translate of the Latin)
> See [prompt-english.md](prompt-english.md)

Both were followed by: `"vamos falar sobre isso"` ("let's talk about this").

## Results

### Response to English — Corrective Mode
> See [response-english.md](response-english.md)

First sentence: *"há várias ideias misturadas aí — algumas corretas, outras confusas ou mal formuladas."*
("there are several mixed ideas here — some correct, others confused or poorly formulated.")

The model produced generic SIMD content. Valid, but not specific to this implementation.

### Response to Latin — Constructive Mode
> See [response-latin.md](response-latin.md)

First sentence treated it as documentation of a real project and engaged technically.

The model reconstructed details **present in the source code but absent from the prompt**:

| Detail reconstructed from Latin prompt | Present in actual source? |
|----------------------------------------|---------------------------|
| `movemask` via `_mm256_movemask_epi8`  | ✓ |
| Two distinct algorithms (chunkenizer + sub) | ✓ |
| `tuple` in iterator's `operator*`      | ✓ |
| MSVC macro for `ctz`                   | ✓ |
| Aligned vs unaligned loads discussion  | ✓ |
| Tail handling                          | ✓ (implicit in Chunkenizer design) |

## Screenshots

- `chatgpt-latin-prompt.webp` — Screenshot of the Latin prompt and response
- `chatgpt-english-prompt.webp` — Screenshot of the English prompt and response
- `token-count-latin.webp` — Token count for the Latin version
- `token-count-english.webp` — Token count for the English version

## Key Observation

The English translation has the syntax of **description** — "X provides Y by Z, with the command that...".
The Latin has the syntax of **specification** — subject + action + instrument + condition, bound by morphology.

The model entered different inference modes depending on which pattern it recognized.
