# Experiment 02 — cppllmcoder: Latin Lapidary Prompt to Gemini

## Context

The [cppllmcoder](https://github.com/Fabio3rs/cppllmcoder) project — a recursive local agent built with C++23, Lua, SQLite, and semantic retrieval — was compressed into a Latin lapidary inscription and sent to Gemini.

## Prompt

> See [prompt-latin.txt](prompt-latin.txt)

Generated using the [lapidary prompt template](../../prompts/lapidary.md).

Thanks to [@eduardoc7](https://github.com/eduardoc7) and Claude Code for the prompt generation.

## Result

Gemini responded in **technical peer-review mode** — not corrective, not auditing the form. It went directly to architecture questions: embedding pipeline, sub-agent communication, context pressure management.

Key quote from Gemini's response:

> *"The Latin inscription of your architecture reveals a sophisticated design philosophy."*

The model recognized the format as an architectural inscription and engaged constructively.

## Significance

This confirms the pattern observed in [Experiment 01](../01-simd-strcmp/): the Latin encoding triggers constructive/technical inference mode rather than corrective/audit mode, across different models (ChatGPT, Claude, Gemini).
