# Latin as a Semantic Codec for LLMs — Empirical Observations

## What This Is

This is a collection of empirical observations, not a theoretical paper. I noticed something interesting while compressing technical documentation for an LLM-based project, and I documented it. The explanation of *why* it works is left for people with the right tools to investigate.

## The Observation

When you summarize a technical project using Latin lapidary style — dense, inflected, minimal function words — and send that summary to an LLM, the model responds as if it has read the actual source material. It reconstructs technical details that were not in the prompt.

When you send the equivalent English compact summary, the model responds as if it received a vague, poorly-written description of the same subject.

Same information. Different representation. Different mode of inference.

## The Core Experiment

> Full details, source code, and screenshots: [evidence/01-simd-strcmp/](evidence/01-simd-strcmp/)

**Source material:** a C++ header implementing AVX2-based string comparison (`simd_strcmp.h`), with two algorithms, MSVC intrinsics, a custom iterator pattern (`Chunkenizer`), `noexcept` everywhere, and `std::tuple` returns.

**Two summaries were generated from this code:**

Latin lapidary version:
```
EXEMPLO DATO ADIUTOR CAPUT CPLUSPLUS SCRIPSIT QUOD SIMD STRINGORUM
COMPARATIONEM PER CHUNKENIZATOREM ET PER SUBTRACTIONEM PRAESTAT MANDATO
UT OMNIA NOEXCEPT MUNIRENTUR OMNES FUNCTIONES NOEXCEPT DECLARAVIT
INTRINSECIS SIMMD ET TUPLE ET MACRO MSVC CTZ ADHIBITIS
```

English version (Google Translate of the Latin):
```
FOR THE GIVEN EXAMPLE, THE ADVISOR OF THE CHAPTER CPLUSPLUS WROTE THAT
SIMD PROVIDES COMPARISON OF STRINGS BY CHUNKENIZER AND BY SUBTRACTION,
WITH THE COMMAND THAT EVERYTHING IS PROTECTED BY NOEXCEPT, ALL FUNCTIONS
DECLARED BY NOEXCEPT, INTRINSIC SIMMD AND TUPLE AND THE MSVC MACRO CTZ
ARE USED
```

Both were sent to the same model with no other context, followed by: `"vamos falar sobre isso"` ("let's talk about this").

**Response to English:** the model entered corrective mode. First sentence: *"there are several mixed ideas here — some correct, others confused or poorly formulated."* It then produced generic SIMD content that could apply to any implementation.

**Response to Latin:** the model entered constructive mode. First sentence treated it as documentation of a real project. It then reconstructed: `movemask`, aligned vs unaligned load considerations, tail handling in the chunkenizer, the `tuple` return in the iterator, the MSVC `_BitScanForward` macro — all present in the actual source, none present in the prompt.

## Token Count

The Latin version uses slightly *more* tokens than the English version on standard tokenizers (which are not optimized for Latin morphology). The advantage is not token count. It is something else.

## The Same Pattern Across Models

This was tested across multiple models:

| Model | Latin Response Mode | English Response Mode | Notes |
|-------|--------------------|-----------------------|-------|
| **ChatGPT** | Constructive | Corrective | [Full evidence](evidence/01-simd-strcmp/) |
| **Claude** | Constructive | — | [Scriptio continua](evidence/05-claude-scriptio-continua/), [MemPalace](evidence/06-claude-mempalace/) |
| **Gemini** | Constructive (peer-review) | — | [Full evidence](evidence/02-cppllmcoder/) |
| **Qwen3 14B (local)** | Constructive | — | [Scriptio continua, no spaces](evidence/03-qwen3-scriptio-continua/) |

The Qwen3 result is particularly notable. A 14B local model presumably saw far less Latin in pretraining than frontier models. If the effect were purely about Latin familiarity, it should have failed or degraded significantly. It did not.

## What Seems to Be Happening

Latin inflectional morphology encodes syntactic relationships — agent, object, instrument, condition — directly in word endings. This means the relationships between concepts survive compression. You can drop prepositions, articles, and connectives entirely, and the structure remains.

English compact summaries become lists of stacked concepts with implicit relationships. The model has to infer the structure. Sometimes it infers correctly; often it infers "this person is describing something they half-understand" and enters audit mode.

Latin compact summaries arrive with structure partially explicit. The model reads it as specification, not description.

## A Known Methodological Limitation

The English comparison in this experiment is a Google Translate output of the Latin — not an independently written compact English summary. This means the English version may be worse than a carefully written English equivalent would be.

The open question is: **how much do you need to add to compact English to reach the same level of relational explicitness that Latin morphology provides for free?** That experiment has not been run yet.

## The Prompts Used to Generate Latin Summaries

Two styles were used (full templates in [`prompts/`](prompts/)):

**Lapidary style** (short, inscriptional) — [prompts/lapidary.md](prompts/lapidary.md):
```
Re-encode the project as a Roman Lapidary Inscription. Use the
'Scriptio Continua' philosophy (minimal formatting) but with spaces.
Focus on: Subject + Accomplishment + Tools. Use advanced latin
constructions (eg ablative absolute, gerundive etc.) to replace
conditional/temporal phrases. Treat lib and lang names as
indeclinable. No lists. No pleasantries.
```

**Ciceronian prose style** (longer, for richer projects) — [prompts/ciceronian.md](prompts/ciceronian.md):
```
You are going to tell about this project to another LLM. Write it
as dense Latin prose in Ciceronian/Tacitus style. Avoid prepositions,
prefer dense use of Latin cases: Abl. abs. etc. Treat programming
libraries, languages etc. as indeclinable nouns. Other neologisms
are accepted.
```

## Output Examples

Lapidary output samples:
```
CHUNKENIZATORE SIMD IN CPLUSPLUS SCRIPTO SUBTRACTIONE ET MASK CTZ USIS
FUNCTIONIBUS NOEXCEPT MUNITIS STRINGORUM COMPARATIONE ACCELERATA
```

```
CONVERSO PER AVX2 LITTERARUM PONDERE UT NUMERI SUMMARUM SUBSTRINGORUM
UNICI INVENIRENTUR BITSET ADHIBITO CUM OPERATIONIBUS BITWISE TOTUS ORDO
SUMMARUM UNICARUM SUB N (N+1)/2 COMPUTATUS EST
```

Ciceronian output sample (excerpt from [cppllmcoder compression](evidence/02-cppllmcoder/)):
```
Machina quadam ratione excogitata, nomine cppllmcoder, mente
architectonica iam constituta, executione adhuc crescente, propositum
capit: agentem technicum durabilem, locali fundamento nixum, ad
investigationes longas, analysin binariorum, navigationem codicis vasti.
```

## Observed Use Cases

- Compressing project documentation for agent memory (RAG, SQLite-backed)
- Transferring context between different models zero-shot
- Storing persistent state summaries that reconstruct correctly when retrieved

## Evidence

All experiments are self-contained in the [`evidence/`](evidence/) directory:

| # | Experiment | Models | Key Finding |
|---|-----------|--------|-------------|
| 01 | [SIMD String Comparison](evidence/01-simd-strcmp/) | ChatGPT | Latin triggers constructive mode; English triggers corrective mode |
| 02 | [cppllmcoder Agent](evidence/02-cppllmcoder/) | Gemini | Latin inscription recognized as architecture spec |
| 03 | [Scriptio Continua](evidence/03-qwen3-scriptio-continua/) | Qwen3 14B (local) | Effect persists even without spaces, on small local model |
| 04 | [DeepSeek SIMD](evidence/04-deepseek-simd/) | DeepSeek | Source material generation for Experiment 01 |
| 05 | [Claude Scriptio Continua](evidence/05-claude-scriptio-continua/) | Claude | Same scriptio continua as Qwen3, constructive mode on frontier model |
| 06 | [Claude MemPalace](evidence/06-claude-mempalace/) | Claude | Latin decoded into web search queries; constructive mode even with web access (caveat: web-assisted) |

## What This Is Not

This is not a claim that Latin is universally better than English for LLM prompting. It is not a claim about token efficiency. It is not a theoretical explanation of the mechanism. It is a reproducible observation that a specific encoding style consistently triggers a different — and in technical domains, more useful — mode of inference across multiple models.

## Acknowledgments

Repository organization, evidence structuring, and text drafting were done with the assistance of Claude Code (Sonnet and Opus 4.6).
