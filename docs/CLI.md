Transformation Types

1. [param] param
2. [eq] --longarg=value
3. [shortarg] -s value
4. [longargval] --longarg value
   ps: param+stack_indexable
   pi: param+stack_iterable
   pp: param+stack_primitive
   pv: param+variant
   es: eq+stack_indexable
   ei
   ep
   ev
   si
   sp
   sv
   s
   stack_iterable, stack_indexable, stack_primitive, variant, stack_bool

```mermaid


/*
 * Key Points
 * Context
 *      A 'dash' moves up the context when inside iterable or indexable
 *      A 'double-dash' moves the context to root
 *      For Iterables Context auto slides to the Iterable subtype and keeps sliding until a non-iterable is encountered.
 *      For Indexable Context slides on named field or --field
 * Primitives
 *      --field=value Only used for primitives inside indexables
 *      After parsing context automatically goes up
 *      If in a list parent context look for comma and split
 * Iterables:
 *      Primitives:
            Can be clubbed into a single arg using comma separator 1,2,3,4
            Can be specified as individual args 1 2 3 4
            Use dash to move up the context
 *      Iterables
 *          Auto context slide down
 *      Indexable
 *          Switch to indexable context
 * Indexable
 *
 */
stateDiagram-v2
    [*] --> stack_indexable : push_indexable

    stack_indexable --> find_key_int_param   : peek_param
    stack_indexable --> stack_indexable      : peek_optval+json_serdes+move_next+error_no_match
    stack_indexable --> find_key_str_longarg : peek_longarg

    stack_iterable --> iterable_add_item : param
    stack_iterable --> iterable_add_item : optval
    stack_iterable --> iterable_add_item : longarg

    stack_primitive --> uplevel : param+assign+move_next
    stack_primitive --> error     : optval+error_missing_value
    stack_primitive --> error     : longarg+error_missing_value

    # check stack for ambiguity
    stack_bool --> uplevel : param+no_match
    stack_bool --> uplevel : param+assign+move_next
    stack_bool --> uplevel : optval
    stack_bool --> uplevel : longarg

    uplevel --> stack_indexable
    uplevel --> stack_iterable: add_stack_item
    uplevel --> variant

    find_key_int_param --> stack_indexable  : push_indexable+move_next
    # TODO : not sure what to do
    find_key_int_param --> stack_iterable   : error
    find_key_int_param --> stack_primitive  : push_primitive
    find_key_int_param --> stack_variant    : push_variant+move_next
    find_key_int_param --> stack_bool       : push_primitive
    find_key_int_param --> uplevel        : no_match

    find_key_str_longarg --> stack_indexable : push_indexable+move_next
    find_key_str_longarg --> stack_iterable  : push_iterable+move_next
    find_key_str_longarg --> stack_primitive : push_primitive+move_next
    find_key_str_longarg --> stack_variant   : push_variant+move_next
    find_key_str_longarg --> stack_bool      : push_bool+move_next
    find_key_str_longarg --> uplevel       : no_match
```
