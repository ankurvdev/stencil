## STATUS : Tentatively Rejected

**Experiment with ConceptNamedTuple which is a subset of ConceptIndexables**

### Motivation

- protocol_cli infers positional/switches in a struct based on default value provided or not
- Other protcols might want to infer whether a default value was provided
- protocols might want to suppress output if a value was default (minimize json for eg)

### Cons:

- Indexables include containers like unordered_maps which have a runtime list of indexes
- default value for dynamic indexable types doesnt make sense
- Burdens needlessly returning false always

### Discussion

Should we have ConceptNamedTuple that is a subset of Indexable

HasDefault
IsDefault
DefaultValue
