; ModuleID = '/Users/max/studing/prog/llvm_course/graph_app/app/abilities/main_ability.c'
source_filename = "/Users/max/studing/prog/llvm_course/graph_app/app/abilities/main_ability.c"
target datalayout = "e-m:o-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-apple-macosx15.0.0"

; Function Attrs: nounwind ssp uwtable(sync)
define range(i32 -245, 246) i32 @getBarrierCenter() local_unnamed_addr #0 {
  %1 = tail call i32 @simRand() #5
  %2 = srem i32 %1, 246
  ret i32 %2
}

declare i32 @simRand(...) local_unnamed_addr #1

; Function Attrs: nounwind ssp uwtable(sync)
define range(i32 -245, 246) i32 @generateNewBarrier(i32 noundef %0, i32 noundef %1, ptr noundef writeonly captures(none) %2) local_unnamed_addr #0 {
  %4 = zext i32 %0 to i64
  %5 = tail call i32 @simRand() #5
  %6 = srem i32 %5, 246
  %7 = icmp sgt i32 %1, 0
  br i1 %7, label %8, label %13

8:                                                ; preds = %3
  %9 = sext i32 %0 to i64
  %10 = getelementptr i32, ptr %2, i64 %9
  %11 = getelementptr i8, ptr %10, i64 -8
  %12 = zext nneg i32 %1 to i64
  br label %14

13:                                               ; preds = %23, %3
  ret i32 %6

14:                                               ; preds = %8, %23
  %15 = phi i64 [ 0, %8 ], [ %24, %23 ]
  %16 = trunc i64 %15 to i32
  %17 = sub i32 %6, %16
  %18 = tail call i32 @simAbs(i32 noundef %17) #5
  %19 = icmp sgt i32 %18, 35
  br i1 %19, label %20, label %23

20:                                               ; preds = %14
  %21 = mul nuw nsw i64 %15, %4
  %22 = getelementptr i32, ptr %11, i64 %21
  store i32 7522568, ptr %22, align 4, !tbaa !6
  br label %23

23:                                               ; preds = %14, %20
  %24 = add nuw nsw i64 %15, 1
  %25 = icmp eq i64 %24, %12
  br i1 %25, label %13, label %14, !llvm.loop !10
}

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(i64 immarg, ptr captures(none)) #2

declare i32 @simAbs(i32 noundef) local_unnamed_addr #1

; Function Attrs: noreturn nounwind ssp uwtable(sync)
define noundef i32 @app() local_unnamed_addr #3 {
  %1 = alloca [256 x [512 x i32]], align 4
  call void @llvm.lifetime.start.p0(i64 524288, ptr nonnull %1) #5
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 4 dereferenceable(524288) %1, i8 0, i64 524288, i1 false)
  %2 = tail call i32 @simRand() #5
  %3 = srem i32 %2, 246
  %4 = sext i32 %3 to i64
  %5 = getelementptr inbounds [256 x [512 x i32]], ptr %1, i64 0, i64 %4, i64 255
  store i32 16763144, ptr %5, align 4, !tbaa !6
  %6 = getelementptr inbounds nuw i8, ptr %1, i64 2040
  br label %7

7:                                                ; preds = %82, %0
  %8 = phi i32 [ %3, %0 ], [ %83, %82 ]
  %9 = phi i32 [ 0, %0 ], [ %84, %82 ]
  %10 = zext i32 %8 to i64
  br label %11

11:                                               ; preds = %7, %34
  %12 = phi i64 [ 0, %7 ], [ %35, %34 ]
  %13 = icmp eq i64 %12, %10
  %14 = freeze i1 %13
  br i1 %14, label %19, label %15

15:                                               ; preds = %11
  %16 = getelementptr inbounds nuw [256 x [512 x i32]], ptr %1, i64 0, i64 %12, i64 1
  %17 = load i32, ptr %16, align 4, !tbaa !6
  %18 = trunc nuw nsw i64 %12 to i32
  br label %21

19:                                               ; preds = %11
  %20 = trunc nuw nsw i64 %12 to i32
  br label %37

21:                                               ; preds = %15, %21
  %22 = phi i32 [ %17, %15 ], [ %29, %21 ]
  %23 = phi i64 [ 1, %15 ], [ %27, %21 ]
  %24 = getelementptr inbounds nuw [256 x [512 x i32]], ptr %1, i64 0, i64 %12, i64 %23
  %25 = trunc i64 %23 to i32
  %26 = add i32 %25, -1
  tail call void @simPutPixel(i32 noundef %26, i32 noundef %18, i32 noundef %22) #5
  %27 = add nuw nsw i64 %23, 1
  %28 = getelementptr inbounds nuw [256 x [512 x i32]], ptr %1, i64 0, i64 %12, i64 %27
  %29 = load i32, ptr %28, align 4, !tbaa !6
  store i32 %29, ptr %24, align 4, !tbaa !6
  %30 = icmp eq i64 %27, 511
  br i1 %30, label %34, label %21, !llvm.loop !12

31:                                               ; preds = %34
  %32 = and i32 %9, 127
  %33 = icmp eq i32 %32, 0
  br i1 %33, label %51, label %82

34:                                               ; preds = %21, %48
  %35 = add nuw nsw i64 %12, 1
  %36 = icmp eq i64 %35, 256
  br i1 %36, label %31, label %11, !llvm.loop !13

37:                                               ; preds = %19, %48
  %38 = phi i64 [ 1, %19 ], [ %49, %48 ]
  %39 = getelementptr inbounds nuw [256 x [512 x i32]], ptr %1, i64 0, i64 %12, i64 %38
  %40 = load i32, ptr %39, align 4, !tbaa !6
  %41 = trunc i64 %38 to i32
  %42 = add i32 %41, -1
  tail call void @simPutPixel(i32 noundef %42, i32 noundef %20, i32 noundef %40) #5
  %43 = icmp eq i64 %38, 447
  br i1 %43, label %48, label %44

44:                                               ; preds = %37
  %45 = add nuw nsw i64 %38, 1
  %46 = getelementptr inbounds nuw [256 x [512 x i32]], ptr %1, i64 0, i64 %12, i64 %45
  %47 = load i32, ptr %46, align 4, !tbaa !6
  store i32 %47, ptr %39, align 4, !tbaa !6
  br label %48

48:                                               ; preds = %37, %44
  %49 = phi i64 [ %45, %44 ], [ 448, %37 ]
  %50 = icmp eq i64 %49, 511
  br i1 %50, label %34, label %37, !llvm.loop !12

51:                                               ; preds = %31
  %52 = tail call i32 @simRand() #5
  %53 = srem i32 %52, 246
  br label %54

54:                                               ; preds = %63, %51
  %55 = phi i64 [ 0, %51 ], [ %64, %63 ]
  %56 = trunc i64 %55 to i32
  %57 = sub i32 %53, %56
  %58 = tail call i32 @simAbs(i32 noundef %57) #5
  %59 = icmp sgt i32 %58, 35
  br i1 %59, label %60, label %63

60:                                               ; preds = %54
  %61 = shl i64 %55, 11
  %62 = getelementptr i8, ptr %6, i64 %61
  store i32 7522568, ptr %62, align 4, !tbaa !6
  br label %63

63:                                               ; preds = %60, %54
  %64 = add nuw nsw i64 %55, 1
  %65 = icmp eq i64 %64, 256
  br i1 %65, label %66, label %54, !llvm.loop !10

66:                                               ; preds = %63
  %67 = tail call i32 @simMin(i32 noundef %8, i32 noundef %53) #5
  %68 = tail call i32 @simMax(i32 noundef %8, i32 noundef %53) #5
  %69 = icmp slt i32 %67, %68
  br i1 %69, label %70, label %72

70:                                               ; preds = %66
  %71 = sext i32 %67 to i64
  br label %75

72:                                               ; preds = %75, %66
  %73 = sext i32 %53 to i64
  %74 = getelementptr inbounds [256 x [512 x i32]], ptr %1, i64 0, i64 %73, i64 447
  store i32 16763144, ptr %74, align 4, !tbaa !6
  br label %82

75:                                               ; preds = %70, %75
  %76 = phi i64 [ %71, %70 ], [ %78, %75 ]
  %77 = getelementptr inbounds [256 x [512 x i32]], ptr %1, i64 0, i64 %76, i64 447
  store i32 16763144, ptr %77, align 4, !tbaa !6
  %78 = add nsw i64 %76, 1
  %79 = tail call i32 @simMax(i32 noundef %8, i32 noundef %53) #5
  %80 = sext i32 %79 to i64
  %81 = icmp slt i64 %78, %80
  br i1 %81, label %75, label %72, !llvm.loop !14

82:                                               ; preds = %72, %31
  %83 = phi i32 [ %53, %72 ], [ %8, %31 ]
  tail call void @simFlush() #5
  %84 = add nuw nsw i32 %9, 1
  br label %7
}

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr writeonly captures(none), i8, i64, i1 immarg) #4

declare void @simPutPixel(i32 noundef, i32 noundef, i32 noundef) local_unnamed_addr #1

declare i32 @simMin(i32 noundef, i32 noundef) local_unnamed_addr #1

declare i32 @simMax(i32 noundef, i32 noundef) local_unnamed_addr #1

declare void @simFlush(...) local_unnamed_addr #1

attributes #0 = { nounwind ssp uwtable(sync) "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+altnzcv,+ccdp,+ccidx,+ccpp,+complxnum,+crc,+dit,+dotprod,+flagm,+fp-armv8,+fp16fml,+fptoint,+fullfp16,+jsconv,+lse,+neon,+pauth,+perfmon,+predres,+ras,+rcpc,+rdm,+sb,+sha2,+sha3,+specrestrict,+ssbs,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8a" }
attributes #1 = { "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+altnzcv,+ccdp,+ccidx,+ccpp,+complxnum,+crc,+dit,+dotprod,+flagm,+fp-armv8,+fp16fml,+fptoint,+fullfp16,+jsconv,+lse,+neon,+pauth,+perfmon,+predres,+ras,+rcpc,+rdm,+sb,+sha2,+sha3,+specrestrict,+ssbs,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8a" }
attributes #2 = { mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #3 = { noreturn nounwind ssp uwtable(sync) "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+altnzcv,+ccdp,+ccidx,+ccpp,+complxnum,+crc,+dit,+dotprod,+flagm,+fp-armv8,+fp16fml,+fptoint,+fullfp16,+jsconv,+lse,+neon,+pauth,+perfmon,+predres,+ras,+rcpc,+rdm,+sb,+sha2,+sha3,+specrestrict,+ssbs,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8a" }
attributes #4 = { mustprogress nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #5 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 2, !"SDK Version", [2 x i32] [i32 15, i32 5]}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{i32 8, !"PIC Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 1}
!4 = !{i32 7, !"frame-pointer", i32 1}
!5 = !{!"Homebrew clang version 21.1.2"}
!6 = !{!7, !7, i64 0}
!7 = !{!"int", !8, i64 0}
!8 = !{!"omnipotent char", !9, i64 0}
!9 = !{!"Simple C/C++ TBAA"}
!10 = distinct !{!10, !11}
!11 = !{!"llvm.loop.mustprogress"}
!12 = distinct !{!12, !11}
!13 = distinct !{!13, !11}
!14 = distinct !{!14, !11}
