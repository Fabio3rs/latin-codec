# Experiment 03 — Qwen3 14B: Scriptio Continua Without Spaces

## Context

A Latin scriptio continua block — **without spaces** — describing the [cppllmcoder](https://github.com/Fabio3rs/cppllmcoder) project was sent to Qwen3 14B running locally via Ollama.

This is the worst-case scenario for tokenization: a 14B local model receiving a wall of Latin text with no whitespace.

## The Prompt

```
AGENSRECURSIVVSLOCALISPRIMVSADCODICVMETFIRMVARIORVMINVESTIGATIONEMCONDITVSINGENTICONTEXTVSVICTOCOMPRESSIONEPERINDIRECTIONEMFACTAPOINTERIBVSCONSERVATISMEMORIAEPERSISTENTISPERSQLiteSTANTELOGICADECISIONVMABLATIVISABSOLVTISPOSITISMODELLISLLMOPERANTIBVSCODICELVAEINSERTOINTRALuaMACHINAMC++23ORCHESTRANTEANALYSIRECURSIVACONTEXTVDIVISORETRIEVALSEMANTICOETSYMBOLICOCONIVNCTISSANDBOXTUTOEXECVTIONEMREGENTEERROREBVSISOLATISSTATVPERMANENTESESSIONSVPERSTITE
```

Followed by: `Expanda + traduza esta Scriptio Continua técnica do Latim`

Followed by (English): `Expand + translate this technical Scriptio Continua from Latin`

## Result

The model correctly reconstructed:
- ✓ Recursive local agent
- ✓ SQLite for persistence
- ✓ Lua + C++23 orchestration
- ✓ Sandbox isolation
- ✓ Permanent sessions
- ✓ Semantic + symbolic retrieval
- ✓ Compression via indirection with pointers

It entered **constructive mode** — expanded and explained, did not correct.

## Notable Artifact

The model treated `ABLATIVISABSOLVTIS` as a technique name ("Lógica de Decisão Ablativa Absoluta") rather than recognizing it as a grammatical construction (ablative absolute). This is ironic and documentable: the model *used* the ablative absolute correctly in its inference but did not *name* it correctly in translation.

**Root cause:** This was a prompt generation artifact, not an inference failure. The model that generated the scriptio continua leaked meta-context ("ablativis absolutis" as a label instead of using the construction). This was fixed in later prompt iterations by allowing spaces.

## Significance

A 14B local model presumably saw far less Latin in pretraining than frontier models. If the effect were purely about Latin corpus familiarity, it should have failed or degraded significantly. **It did not.** This strengthens the structural argument over the familiarity argument.
