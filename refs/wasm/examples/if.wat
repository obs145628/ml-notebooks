(module
 (type $none_=>_none (func))
 (type $i32_=>_i32 (func (param i32) (result i32)))
 (type $i32_i32_=>_i32 (func (param i32 i32) (result i32)))
 (global $global$0 i32 (i32.const 0))
 (export "__wasm_apply_relocs" (func $1))
 (export "foo" (func $2))
 (export "bar" (func $3))
 (export "__dso_handle" (global $global$0))
 (export "__post_instantiate" (func $4))
 (func $0 (; 0 ;)
  (call $1)
 )
 (func $1 (; 1 ;)
  (nop)
 )
 (func $2 (; 2 ;) (param $0 i32) (result i32)
  (local $1 i32)
  (local.set $1
   (i32.shl
    (local.get $0)
    (i32.const 1)
   )
  )
  (if
   (i32.ge_s
    (local.get $0)
    (i32.const 11)
   )
   (block
    (if
     (i32.lt_s
      (i32.mul
       (local.get $0)
       (i32.const 3)
      )
      (local.get $0)
     )
     (return
      (i32.add
       (i32.add
        (local.get $0)
        (i32.const 5)
       )
       (local.get $1)
      )
     )
    )
    (return
     (i32.add
      (select
       (i32.mul
        (local.get $0)
        (i32.const 100)
       )
       (local.get $0)
       (i32.lt_s
        (local.get $0)
        (i32.const 3)
       )
      )
      (local.get $1)
     )
    )
   )
  )
  (i32.add
   (i32.mul
    (i32.add
     (local.get $1)
     (i32.const 67)
    )
    (local.get $0)
   )
   (local.get $1)
  )
 )
 (func $3 (; 3 ;) (param $0 i32) (param $1 i32) (result i32)
  (if
   (i32.le_s
    (i32.or
     (local.get $0)
     (local.get $1)
    )
    (i32.const -1)
   )
   (return
    (i32.add
     (local.get $0)
     (local.get $1)
    )
   )
  )
  (select
   (select
    (local.get $1)
    (i32.const 100)
    (i32.lt_s
     (local.get $0)
     (local.get $1)
    )
   )
   (local.get $0)
   (i32.le_s
    (local.get $0)
    (local.get $1)
   )
  )
 )
 (func $4 (; 4 ;)
  (call $0)
 )
 ;; custom section "dylink", size 5
)

