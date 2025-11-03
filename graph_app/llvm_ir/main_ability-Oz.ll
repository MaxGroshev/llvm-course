; ModuleID = '/Users/max/studing/prog/llvm_course/graph_app/app/abilities/main_ability.c'
source_filename = "/Users/max/studing/prog/llvm_course/graph_app/app/abilities/main_ability.c"
target datalayout = "e-m:o-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-apple-macosx15.0.0"

; Function Attrs: minsize noreturn nounwind optsize ssp uwtable(sync)
define noundef i32 @app() local_unnamed_addr #0 {
  %1 = alloca [256 x [512 x i32]], align 4
  call void @llvm.lifetime.start.p0(i64 524288, ptr nonnull %1) #4
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 4 dereferenceable(524288) %1, i8 0, i64 524288, i1 false)
  %2 = tail call i32 @simRand() #5
  %3 = srem i32 %2, 246
  %4 = sext i32 %3 to i64
  %5 = getelementptr inbounds [256 x [512 x i32]], ptr %1, i64 0, i64 %4, i64 255
  store i32 16763144, ptr %5, align 4, !tbaa !6
  %6 = getelementptr inbounds nuw i8, ptr %1, i64 2040
  br label %7

7:                                                ; preds = %68, %0
  %8 = phi i32 [ %3, %0 ], [ %69, %68 ]
  %9 = phi i32 [ 0, %0 ], [ %70, %68 ]
  %10 = zext i32 %8 to i64
  br label %11

11:                                               ; preds = %23, %7
  %12 = phi i64 [ %24, %23 ], [ 0, %7 ]
  %13 = icmp eq i64 %12, 256
  br i1 %13, label %17, label %14

14:                                               ; preds = %11
  %15 = icmp eq i64 %12, %10
  %16 = trunc nuw nsw i64 %12 to i32
  br label %20

17:                                               ; preds = %11
  %18 = and i32 %9, 127
  %19 = icmp eq i32 %18, 0
  br i1 %19, label %38, label %68

20:                                               ; preds = %32, %14
  %21 = phi i64 [ 1, %14 ], [ %33, %32 ]
  %22 = icmp eq i64 %21, 511
  br i1 %22, label %23, label %25

23:                                               ; preds = %20
  %24 = add nuw nsw i64 %12, 1
  br label %11, !llvm.loop !10

25:                                               ; preds = %20
  %26 = getelementptr inbounds nuw [256 x [512 x i32]], ptr %1, i64 0, i64 %12, i64 %21
  %27 = load i32, ptr %26, align 4, !tbaa !6
  %28 = trunc i64 %21 to i32
  %29 = add i32 %28, -1
  tail call void @simPutPixel(i32 noundef %29, i32 noundef %16, i32 noundef %27) #5
  %30 = icmp eq i64 %21, 447
  %31 = select i1 %30, i1 %15, i1 false
  br i1 %31, label %32, label %34

32:                                               ; preds = %25, %34
  %33 = phi i64 [ %35, %34 ], [ 448, %25 ]
  br label %20, !llvm.loop !12

34:                                               ; preds = %25
  %35 = add nuw nsw i64 %21, 1
  %36 = getelementptr inbounds nuw [256 x [512 x i32]], ptr %1, i64 0, i64 %12, i64 %35
  %37 = load i32, ptr %36, align 4, !tbaa !6
  store i32 %37, ptr %26, align 4, !tbaa !6
  br label %32

38:                                               ; preds = %17
  %39 = tail call i32 @simRand() #5
  %40 = srem i32 %39, 246
  %41 = sext i32 %40 to i64
  br label %42

42:                                               ; preds = %53, %38
  %43 = phi i64 [ %54, %53 ], [ 0, %38 ]
  %44 = icmp eq i64 %43, 256
  br i1 %44, label %55, label %45

45:                                               ; preds = %42
  %46 = trunc i64 %43 to i32
  %47 = sub i32 %40, %46
  %48 = tail call i32 @simAbs(i32 noundef %47) #5
  %49 = icmp sgt i32 %48, 35
  br i1 %49, label %50, label %53

50:                                               ; preds = %45
  %51 = shl i64 %43, 11
  %52 = getelementptr i8, ptr %6, i64 %51
  store i32 7522568, ptr %52, align 4, !tbaa !6
  br label %53

53:                                               ; preds = %50, %45
  %54 = add nuw nsw i64 %43, 1
  br label %42, !llvm.loop !13

55:                                               ; preds = %42
  %56 = tail call i32 @simMin(i32 noundef %8, i32 noundef %40) #5
  %57 = sext i32 %56 to i64
  br label %58

58:                                               ; preds = %65, %55
  %59 = phi i64 [ %67, %65 ], [ %57, %55 ]
  %60 = tail call i32 @simMax(i32 noundef %8, i32 noundef %40) #5
  %61 = sext i32 %60 to i64
  %62 = icmp slt i64 %59, %61
  br i1 %62, label %65, label %63

63:                                               ; preds = %58
  %64 = getelementptr inbounds [256 x [512 x i32]], ptr %1, i64 0, i64 %41, i64 447
  store i32 16763144, ptr %64, align 4, !tbaa !6
  br label %68

65:                                               ; preds = %58
  %66 = getelementptr inbounds [256 x [512 x i32]], ptr %1, i64 0, i64 %59, i64 447
  store i32 16763144, ptr %66, align 4, !tbaa !6
  %67 = add nsw i64 %59, 1
  br label %58, !llvm.loop !14

68:                                               ; preds = %63, %17
  %69 = phi i32 [ %40, %63 ], [ %8, %17 ]
  tail call void @simFlush() #5
  %70 = add nuw nsw i32 %9, 1
  br label %7
}

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(i64 immarg, ptr nocapture) #1

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #2

; Function Attrs: minsize optsize
declare void @simPutPixel(i32 noundef, i32 noundef, i32 noundef) local_unnamed_addr #3

; Function Attrs: minsize optsize
declare i32 @simMin(i32 noundef, i32 noundef) local_unnamed_addr #3

; Function Attrs: minsize optsize
declare i32 @simMax(i32 noundef, i32 noundef) local_unnamed_addr #3

; Function Attrs: minsize optsize
declare void @simFlush(...) local_unnamed_addr #3

; Function Attrs: minsize optsize
declare i32 @simRand(...) local_unnamed_addr #3

; Function Attrs: minsize optsize
declare i32 @simAbs(i32 noundef) local_unnamed_addr #3

attributes #0 = { minsize noreturn nounwind optsize ssp uwtable(sync) "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+altnzcv,+ccdp,+ccidx,+ccpp,+complxnum,+crc,+dit,+dotprod,+flagm,+fp-armv8,+fp16fml,+fptoint,+fullfp16,+jsconv,+lse,+neon,+pauth,+perfmon,+predres,+ras,+rcpc,+rdm,+sb,+sha2,+sha3,+specrestrict,+ssbs,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8a,+zcm,+zcz" }
attributes #1 = { mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { mustprogress nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #3 = { minsize optsize "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+altnzcv,+ccdp,+ccidx,+ccpp,+complxnum,+crc,+dit,+dotprod,+flagm,+fp-armv8,+fp16fml,+fptoint,+fullfp16,+jsconv,+lse,+neon,+pauth,+perfmon,+predres,+ras,+rcpc,+rdm,+sb,+sha2,+sha3,+specrestrict,+ssbs,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8a,+zcm,+zcz" }
attributes #4 = { nounwind }
attributes #5 = { minsize nounwind optsize }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 2, !"SDK Version", [2 x i32] [i32 15, i32 5]}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{i32 8, !"PIC Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 1}
!4 = !{i32 7, !"frame-pointer", i32 1}
!5 = !{!"Homebrew clang version 20.1.8"}
!6 = !{!7, !7, i64 0}
!7 = !{!"int", !8, i64 0}
!8 = !{!"omnipotent char", !9, i64 0}
!9 = !{!"Simple C/C++ TBAA"}
!10 = distinct !{!10, !11}
!11 = !{!"llvm.loop.mustprogress"}
!12 = distinct !{!12, !11}
!13 = distinct !{!13, !11}
!14 = distinct !{!14, !11}
