; ModuleID = '/Users/max/studing/prog/llvm_course/graph_app/app/abilities/main_ability.c'
source_filename = "/Users/max/studing/prog/llvm_course/graph_app/app/abilities/main_ability.c"
target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-apple-macosx15.0.0"

; Function Attrs: minsize nounwind optsize ssp uwtable(sync)
define range(i32 -245, 246) i32 @getBarrierCenter() local_unnamed_addr #0 {
  %1 = tail call i32 @simRand() #6
  %2 = srem i32 %1, 246
  ret i32 %2
}

; Function Attrs: minsize optsize
declare i32 @simRand(...) local_unnamed_addr #1

; Function Attrs: minsize nounwind optsize ssp uwtable(sync)
define range(i32 -245, 246) i32 @generateNewBarrier(i32 noundef %0, i32 noundef %1, ptr nocapture noundef writeonly %2) local_unnamed_addr #0 {
  %4 = zext i32 %0 to i64
  %5 = tail call i32 @simRand() #6
  %6 = srem i32 %5, 246
  %7 = sext i32 %0 to i64
  %8 = getelementptr i32, ptr %2, i64 %7
  %9 = getelementptr i8, ptr %8, i64 -8
  %10 = tail call i32 @llvm.smax.i32(i32 %1, i32 0)
  %11 = zext nneg i32 %10 to i64
  br label %12

12:                                               ; preds = %24, %3
  %13 = phi i64 [ %25, %24 ], [ 0, %3 ]
  %14 = icmp eq i64 %13, %11
  br i1 %14, label %15, label %16

15:                                               ; preds = %12
  ret i32 %6

16:                                               ; preds = %12
  %17 = trunc i64 %13 to i32
  %18 = sub i32 %6, %17
  %19 = tail call i32 @simAbs(i32 noundef %18) #6
  %20 = icmp sgt i32 %19, 35
  br i1 %20, label %21, label %24

21:                                               ; preds = %16
  %22 = mul nuw nsw i64 %13, %4
  %23 = getelementptr i32, ptr %9, i64 %22
  store i32 7522568, ptr %23, align 4, !tbaa !6
  br label %24

24:                                               ; preds = %16, %21
  %25 = add nuw nsw i64 %13, 1
  br label %12, !llvm.loop !10
}

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(i64 immarg, ptr nocapture) #2

; Function Attrs: minsize optsize
declare i32 @simAbs(i32 noundef) local_unnamed_addr #1

; Function Attrs: minsize noreturn nounwind optsize ssp uwtable(sync)
define noundef i32 @app() local_unnamed_addr #3 {
  %1 = alloca [256 x [512 x i32]], align 4
  call void @llvm.lifetime.start.p0(i64 524288, ptr nonnull %1) #7
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 4 dereferenceable(524288) %1, i8 0, i64 524288, i1 false)
  %2 = tail call i32 @simRand() #6
  %3 = srem i32 %2, 246
  %4 = sext i32 %3 to i64
  %5 = getelementptr inbounds [256 x [512 x i32]], ptr %1, i64 0, i64 %4, i64 255
  store i32 16763144, ptr %5, align 4, !tbaa !6
  br label %6

6:                                                ; preds = %52, %0
  %7 = phi i32 [ %3, %0 ], [ %53, %52 ]
  %8 = phi i32 [ 0, %0 ], [ %54, %52 ]
  %9 = zext i32 %7 to i64
  br label %10

10:                                               ; preds = %22, %6
  %11 = phi i64 [ %23, %22 ], [ 0, %6 ]
  %12 = icmp eq i64 %11, 256
  br i1 %12, label %16, label %13

13:                                               ; preds = %10
  %14 = icmp eq i64 %11, %9
  %15 = trunc nuw nsw i64 %11 to i32
  br label %19

16:                                               ; preds = %10
  %17 = and i32 %8, 127
  %18 = icmp eq i32 %17, 0
  br i1 %18, label %37, label %52

19:                                               ; preds = %31, %13
  %20 = phi i64 [ 1, %13 ], [ %32, %31 ]
  %21 = icmp eq i64 %20, 511
  br i1 %21, label %22, label %24

22:                                               ; preds = %19
  %23 = add nuw nsw i64 %11, 1
  br label %10, !llvm.loop !12

24:                                               ; preds = %19
  %25 = getelementptr inbounds [256 x [512 x i32]], ptr %1, i64 0, i64 %11, i64 %20
  %26 = load i32, ptr %25, align 4, !tbaa !6
  %27 = trunc i64 %20 to i32
  %28 = add i32 %27, -1
  tail call void @simPutPixel(i32 noundef %28, i32 noundef %15, i32 noundef %26) #6
  %29 = icmp eq i64 %20, 447
  %30 = select i1 %29, i1 %14, i1 false
  br i1 %30, label %31, label %33

31:                                               ; preds = %24, %33
  %32 = phi i64 [ %34, %33 ], [ 448, %24 ]
  br label %19, !llvm.loop !13

33:                                               ; preds = %24
  %34 = add nuw nsw i64 %20, 1
  %35 = getelementptr inbounds [256 x [512 x i32]], ptr %1, i64 0, i64 %11, i64 %34
  %36 = load i32, ptr %35, align 4, !tbaa !6
  store i32 %36, ptr %25, align 4, !tbaa !6
  br label %31

37:                                               ; preds = %16
  %38 = call i32 @generateNewBarrier(i32 noundef 512, i32 noundef 256, ptr noundef nonnull %1) #8
  %39 = tail call i32 @simMin(i32 noundef %7, i32 noundef %38) #6
  %40 = sext i32 %39 to i64
  br label %41

41:                                               ; preds = %49, %37
  %42 = phi i64 [ %51, %49 ], [ %40, %37 ]
  %43 = tail call i32 @simMax(i32 noundef %7, i32 noundef %38) #6
  %44 = sext i32 %43 to i64
  %45 = icmp slt i64 %42, %44
  br i1 %45, label %49, label %46

46:                                               ; preds = %41
  %47 = sext i32 %38 to i64
  %48 = getelementptr inbounds [256 x [512 x i32]], ptr %1, i64 0, i64 %47, i64 447
  store i32 16763144, ptr %48, align 4, !tbaa !6
  br label %52

49:                                               ; preds = %41
  %50 = getelementptr inbounds [256 x [512 x i32]], ptr %1, i64 0, i64 %42, i64 447
  store i32 16763144, ptr %50, align 4, !tbaa !6
  %51 = add nsw i64 %42, 1
  br label %41, !llvm.loop !14

52:                                               ; preds = %46, %16
  %53 = phi i32 [ %38, %46 ], [ %7, %16 ]
  tail call void @simFlush() #6
  %54 = add nuw nsw i32 %8, 1
  br label %6
}

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #4

; Function Attrs: minsize optsize
declare void @simPutPixel(i32 noundef, i32 noundef, i32 noundef) local_unnamed_addr #1

; Function Attrs: minsize optsize
declare i32 @simMin(i32 noundef, i32 noundef) local_unnamed_addr #1

; Function Attrs: minsize optsize
declare i32 @simMax(i32 noundef, i32 noundef) local_unnamed_addr #1

; Function Attrs: minsize optsize
declare void @simFlush(...) local_unnamed_addr #1

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.smax.i32(i32, i32) #5

attributes #0 = { minsize nounwind optsize ssp uwtable(sync) "frame-pointer"="non-leaf" "no-trapping-math"="true" "probe-stack"="__chkstk_darwin" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+altnzcv,+bti,+ccdp,+ccidx,+complxnum,+crc,+dit,+dotprod,+flagm,+fp-armv8,+fp16fml,+fptoint,+fullfp16,+jsconv,+lse,+neon,+pauth,+perfmon,+predres,+ras,+rcpc,+rdm,+sb,+sha2,+sha3,+specrestrict,+ssbs,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8.5a,+v8a,+zcm,+zcz" }
attributes #1 = { minsize optsize "frame-pointer"="non-leaf" "no-trapping-math"="true" "probe-stack"="__chkstk_darwin" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+altnzcv,+bti,+ccdp,+ccidx,+complxnum,+crc,+dit,+dotprod,+flagm,+fp-armv8,+fp16fml,+fptoint,+fullfp16,+jsconv,+lse,+neon,+pauth,+perfmon,+predres,+ras,+rcpc,+rdm,+sb,+sha2,+sha3,+specrestrict,+ssbs,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8.5a,+v8a,+zcm,+zcz" }
attributes #2 = { mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #3 = { minsize noreturn nounwind optsize ssp uwtable(sync) "frame-pointer"="non-leaf" "no-trapping-math"="true" "probe-stack"="__chkstk_darwin" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+altnzcv,+bti,+ccdp,+ccidx,+complxnum,+crc,+dit,+dotprod,+flagm,+fp-armv8,+fp16fml,+fptoint,+fullfp16,+jsconv,+lse,+neon,+pauth,+perfmon,+predres,+ras,+rcpc,+rdm,+sb,+sha2,+sha3,+specrestrict,+ssbs,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8.5a,+v8a,+zcm,+zcz" }
attributes #4 = { mustprogress nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #5 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #6 = { minsize nounwind optsize }
attributes #7 = { nounwind }
attributes #8 = { minsize optsize }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 2, !"SDK Version", [2 x i32] [i32 15, i32 5]}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{i32 8, !"PIC Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 1}
!4 = !{i32 7, !"frame-pointer", i32 1}
!5 = !{!"Apple clang version 17.0.0 (clang-1700.0.13.5)"}
!6 = !{!7, !7, i64 0}
!7 = !{!"int", !8, i64 0}
!8 = !{!"omnipotent char", !9, i64 0}
!9 = !{!"Simple C/C++ TBAA"}
!10 = distinct !{!10, !11}
!11 = !{!"llvm.loop.mustprogress"}
!12 = distinct !{!12, !11}
!13 = distinct !{!13, !11}
!14 = distinct !{!14, !11}
