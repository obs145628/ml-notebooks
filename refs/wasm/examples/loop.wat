(module
 (type $none_=>_none (func))
 (type $i32_=>_i32 (func (param i32) (result i32)))
 (type $i32_=>_none (func (param i32)))
 (import "env" "foo" (func $fimport$0))
 (global $global$0 i32 (i32.const 0))
 (export "__wasm_apply_relocs" (func $1))
 (export "foo_n" (func $2))
 (export "fact" (func $3))
 (export "el_break" (func $4))
 (export "el_continue" (func $5))
 (export "__dso_handle" (global $global$0))
 (export "__post_instantiate" (func $6))
 (func $0 (; 1 ;)
  (call $1)
 )
 (func $1 (; 2 ;)
  (nop)
 )
 (func $2 (; 3 ;) (param $0 i32)
  (local $1 i32)
  (local.set $1
   (i32.const 0)
  )
  (if
   (i32.gt_s
    (local.get $0)
    (i32.const 0)
   )
   (loop $label$2
    (call $fimport$0)
    (br_if $label$2
     (i32.ne
      (local.tee $1
       (i32.add
        (local.get $1)
        (i32.const 1)
       )
      )
      (local.get $0)
     )
    )
   )
  )
 )
 (func $3 (; 4 ;) (param $0 i32) (result i32)
  (local $1 i32)
  (local $2 i32)
  (local.set $1
   (i32.const 1)
  )
  (if
   (i32.ge_s
    (local.get $0)
    (i32.const 2)
   )
   (loop $label$2
    (local.set $1
     (i32.mul
      (local.get $0)
      (local.get $1)
     )
    )
    (local.set $2
     (i32.gt_s
      (local.get $0)
      (i32.const 2)
     )
    )
    (local.set $0
     (i32.add
      (local.get $0)
      (i32.const -1)
     )
    )
    (br_if $label$2
     (local.get $2)
    )
   )
  )
  (local.get $1)
 )
 (func $4 (; 5 ;) (param $0 i32) (result i32)
  (local $1 i32)
  (local $2 i32)
  (local.set $1
   (i32.const 0)
  )
  (block $label$1
   (br_if $label$1
    (i32.ge_s
     (local.tee $2
      (i32.div_s
       (local.get $0)
       (i32.const 3)
      )
     )
     (local.get $0)
    )
   )
   (br_if $label$1
    (i32.eqz
     (i32.and
      (local.get $2)
      (i32.const 3)
     )
    )
   )
   (local.set $1
    (i32.add
     (select
      (local.tee $0
       (i32.add
        (local.tee $1
         (i32.xor
          (local.get $2)
          (i32.const -1)
         )
        )
        (local.get $0)
       )
      )
      (local.tee $1
       (i32.and
        (local.get $1)
        (i32.const 3)
       )
      )
      (i32.lt_u
       (local.get $0)
       (local.get $1)
      )
     )
     (i32.const 1)
    )
   )
  )
  (local.get $1)
 )
 (func $5 (; 6 ;) (param $0 i32) (result i32)
  (local $1 i32)
  (local $2 i32)
  (local.set $1
   (i32.const 0)
  )
  (if
   (i32.lt_s
    (local.tee $2
     (i32.div_s
      (local.get $0)
      (i32.const 3)
     )
    )
    (local.get $0)
   )
   (loop $label$2
    (local.set $1
     (i32.add
      (local.get $1)
      (i32.ne
       (i32.and
        (local.get $2)
        (i32.const 3)
       )
       (i32.const 0)
      )
     )
    )
    (br_if $label$2
     (i32.ne
      (local.tee $2
       (i32.add
        (local.get $2)
        (i32.const 1)
       )
      )
      (local.get $0)
     )
    )
   )
  )
  (local.get $1)
 )
 (func $6 (; 7 ;)
  (call $0)
 )
 ;; custom section "dylink", size 5
)

