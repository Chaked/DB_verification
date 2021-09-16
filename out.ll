; ModuleID = 'harness'
source_filename = "harness"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"

%struct._list = type { %struct._list*, i8* }

@0 = private constant [1 x i32] [i32 1]
@1 = private global i32 0
@2 = private constant [1 x i8*] [i8* inttoptr (i64 126 to i8*)]
@3 = private global i32 0
@4 = private constant [1 x %struct._list*] zeroinitializer
@5 = private global i32 0
@6 = private constant [1 x i32] [i32 2]
@7 = private global i32 0

define i32 @nd() {
entry:
  %0 = load i32, i32* @1
  %1 = add i32 %0, 1
  store i32 %1, i32* @1
  %2 = call i32 @__seahorn_get_value_i32(i32 %0, i32* getelementptr inbounds ([1 x i32], [1 x i32]* @0, i32 0, i32 0), i32 1)
  ret i32 %2
}

declare i32 @__seahorn_get_value_i32(i32, i32*, i32)

define i8* @nd_ptr() {
entry:
  %0 = load i32, i32* @3
  %1 = add i32 %0, 1
  store i32 %1, i32* @3
  %2 = call i8* @__seahorn_get_value_ptr(i32 %0, i8* bitcast ([1 x i8*]* @2 to i8*), i32 1, i32 8)
  ret i8* %2
}

declare i8* @__seahorn_get_value_ptr(i32, i8*, i32, i32)

define %struct._list* @verifier.nondet.1() {
entry:
  %0 = load i32, i32* @5
  %1 = add i32 %0, 1
  store i32 %1, i32* @5
  %2 = call %struct._list* bitcast (i8* (i32, i8*, i32, i32)* @__seahorn_get_value_ptr to %struct._list* (i32, i8*, i32, i32)*)(i32 %0, i8* bitcast ([1 x %struct._list*]* @4 to i8*), i32 1, i32 128)
  ret %struct._list* %2
}

define i32 @verifier.nondet.2() {
entry:
  %0 = load i32, i32* @7
  %1 = add i32 %0, 1
  store i32 %1, i32* @7
  %2 = call i32 @__seahorn_get_value_i32(i32 %0, i32* getelementptr inbounds ([1 x i32], [1 x i32]* @6, i32 0, i32 0), i32 1)
  ret i32 %2
}

; Function Attrs: noinline
declare i8* @__emv(i8*) #0

define void @__seahorn_mem_init_routine() {
entry:
  call void @__seahorn_mem_alloc(i8* null, i8* null, i64 3908, i64 8)
  call void @__seahorn_mem_alloc(i8* null, i8* null, i64 3904, i64 8)
  call void @__seahorn_mem_alloc(i8* null, i8* null, i64 297, i64 8)
  call void @__seahorn_mem_alloc(i8* null, i8* null, i64 2144, i64 8)
  call void @__seahorn_mem_alloc(i8* null, i8* null, i64 2146, i64 8)
  call void @__seahorn_mem_alloc(i8* null, i8* null, i64 2366, i64 8)
  call void @__seahorn_mem_alloc(i8* null, i8* null, i64 2368, i64 8)
  ret void
}

declare void @__seahorn_mem_alloc(i8*, i8*, i64, i64)

declare void @__seahorn_mem_init(i8*, i64, i64)

attributes #0 = { noinline }
