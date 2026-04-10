# TODO

## Experiments to Run

### Fair English Comparison
The current English baseline is a Google Translate output — not an independently written compact English summary. A proper comparison needs:
- Compact English with explicit syntactic role annotations (e.g., `strings[obj]`, `subtraction[instr]`)
- Measure: how many tokens must be added to English to match Latin's relational explicitness?
- This is probably the most original contribution of the work

### Automated Benchmarks
- Systematic testing across prompt sizes (short lapidary, medium, long Ciceronian)
- Multiple models (ChatGPT, Claude, Gemini, Qwen3, DeepSeek, Llama, etc.)
- Multiple domains (C++/systems, web, databases, algorithms, non-technical)
- Controlled variables: same information, same follow-up prompt
- Quantifiable metrics beyond qualitative "corrective vs constructive" (e.g., count of accurate technical details reconstructed, count of corrections/hedges in response)

### Cross-Model Comparison Matrix
A spreadsheet/table crossing:
- **Rows:** prompt format (natural English / compact English / annotated English / Latin lapidary / Latin Ciceronian / scriptio continua)
- **Columns:** model (ChatGPT / Claude / Gemini / DeepSeek / Qwen3 14B / Llama / etc.)
- **Cells:** inference mode observed (constructive / corrective / neutral), technical accuracy score

Key question: does the ChatGPT "hostile correction mode" on non-canonical text appear in other models, or is it model-specific?

## Missing Evidence

### Claude Evidence
~~The README claims "same pattern" for Claude but there is no evidence folder yet.~~ ✅ Done — see `evidence/05-claude-scriptio-continua/` and `evidence/06-claude-mempalace/`

### Gemini Visual Evidence
The `evidence/02-cppllmcoder/` folder has no screenshot of Gemini's response. Needs:
- Screenshot or full response text export

## Documentation

### Prompt Generation Artifacts
Document the known failure mode: when the generating model leaks meta-context into the Latin output (e.g., `ABLATIVISABSOLVTIS` as a label instead of using the construction). Include:
- Which model generated it
- Which prompt version
- The fix (allowing spaces)
