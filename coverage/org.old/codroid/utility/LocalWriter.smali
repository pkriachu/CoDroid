.class public Lorg/codroid/utility/LocalWriter;
.super Lorg/codroid/utility/Statistics;
.source "LocalWriter.java"


# static fields
.field private static myTask:Lorg/codroid/utility/StatisticsTask;


# direct methods
.method static constructor <clinit>()V
    .locals 1

    .prologue
    .line 7
    const/4 v0, 0x0

    sput-object v0, Lorg/codroid/utility/LocalWriter;->myTask:Lorg/codroid/utility/StatisticsTask;

    return-void
.end method

.method public constructor <init>()V
    .locals 0

    .prologue
    .line 6
    invoke-direct {p0}, Lorg/codroid/utility/Statistics;-><init>()V

    return-void
.end method

.method public static commit()V
    .locals 0

    .prologue
    .line 17
    invoke-static {}, Lorg/codroid/utility/LocalWriter;->setupFlush()V

    .line 18
    invoke-static {}, Lorg/codroid/utility/Statistics;->commit()V

    .line 19
    return-void
.end method

.method public static commit(J)V
    .locals 0
    .param p0, "v"    # J

    .prologue
    .line 22
    invoke-static {}, Lorg/codroid/utility/LocalWriter;->setupFlush()V

    .line 23
    invoke-static {p0, p1}, Lorg/codroid/utility/Statistics;->commit(J)V

    .line 24
    return-void
.end method

.method public static declared-synchronized setupFlush()V
    .locals 2

    .prologue
    .line 10
    const-class v1, Lorg/codroid/utility/LocalWriter;

    monitor-enter v1

    :try_start_0
    sget-object v0, Lorg/codroid/utility/LocalWriter;->myTask:Lorg/codroid/utility/StatisticsTask;

    if-nez v0, :cond_0

    .line 11
    new-instance v0, Lorg/codroid/utility/LocalWriterTask;

    invoke-direct {v0}, Lorg/codroid/utility/LocalWriterTask;-><init>()V

    sput-object v0, Lorg/codroid/utility/LocalWriter;->myTask:Lorg/codroid/utility/StatisticsTask;

    .line 12
    sget-object v0, Lorg/codroid/utility/LocalWriter;->myTask:Lorg/codroid/utility/StatisticsTask;

    invoke-static {v0}, Lorg/codroid/utility/LocalWriter;->setStatisticsTask(Lorg/codroid/utility/StatisticsTask;)V
    :try_end_0
    .catchall {:try_start_0 .. :try_end_0} :catchall_0

    .line 14
    :cond_0
    monitor-exit v1

    return-void

    .line 10
    :catchall_0
    move-exception v0

    monitor-exit v1

    throw v0
.end method

.method public static value0C()V
    .locals 2

    .prologue
    .line 26
    const-wide/16 v0, 0x0

    invoke-static {v0, v1}, Lorg/codroid/utility/LocalWriter;->updateValue(J)V

    invoke-static {}, Lorg/codroid/utility/LocalWriter;->commit()V

    return-void
.end method

.method public static value1C()V
    .locals 2

    .prologue
    .line 27
    const-wide/16 v0, 0x1

    invoke-static {v0, v1}, Lorg/codroid/utility/LocalWriter;->updateValue(J)V

    invoke-static {}, Lorg/codroid/utility/LocalWriter;->commit()V

    return-void
.end method

.method public static value2C()V
    .locals 2

    .prologue
    .line 28
    const-wide/16 v0, 0x2

    invoke-static {v0, v1}, Lorg/codroid/utility/LocalWriter;->updateValue(J)V

    invoke-static {}, Lorg/codroid/utility/LocalWriter;->commit()V

    return-void
.end method

.method public static value3C()V
    .locals 2

    .prologue
    .line 29
    const-wide/16 v0, 0x3

    invoke-static {v0, v1}, Lorg/codroid/utility/LocalWriter;->updateValue(J)V

    invoke-static {}, Lorg/codroid/utility/LocalWriter;->commit()V

    return-void
.end method

.method public static value4C()V
    .locals 2

    .prologue
    .line 30
    const-wide/16 v0, 0x4

    invoke-static {v0, v1}, Lorg/codroid/utility/LocalWriter;->updateValue(J)V

    invoke-static {}, Lorg/codroid/utility/LocalWriter;->commit()V

    return-void
.end method

.method public static value5C()V
    .locals 2

    .prologue
    .line 31
    const-wide/16 v0, 0x5

    invoke-static {v0, v1}, Lorg/codroid/utility/LocalWriter;->updateValue(J)V

    invoke-static {}, Lorg/codroid/utility/LocalWriter;->commit()V

    return-void
.end method

.method public static value6C()V
    .locals 2

    .prologue
    .line 32
    const-wide/16 v0, 0x6

    invoke-static {v0, v1}, Lorg/codroid/utility/LocalWriter;->updateValue(J)V

    invoke-static {}, Lorg/codroid/utility/LocalWriter;->commit()V

    return-void
.end method

.method public static value7C()V
    .locals 2

    .prologue
    .line 33
    const-wide/16 v0, 0x7

    invoke-static {v0, v1}, Lorg/codroid/utility/LocalWriter;->updateValue(J)V

    invoke-static {}, Lorg/codroid/utility/LocalWriter;->commit()V

    return-void
.end method

.method public static value8C()V
    .locals 2

    .prologue
    .line 34
    const-wide/16 v0, 0x8

    invoke-static {v0, v1}, Lorg/codroid/utility/LocalWriter;->updateValue(J)V

    invoke-static {}, Lorg/codroid/utility/LocalWriter;->commit()V

    return-void
.end method

.method public static value9C()V
    .locals 2

    .prologue
    .line 35
    const-wide/16 v0, 0x9

    invoke-static {v0, v1}, Lorg/codroid/utility/LocalWriter;->updateValue(J)V

    invoke-static {}, Lorg/codroid/utility/LocalWriter;->commit()V

    return-void
.end method

.method public static valueAC()V
    .locals 2

    .prologue
    .line 36
    const-wide/16 v0, 0xa

    invoke-static {v0, v1}, Lorg/codroid/utility/LocalWriter;->updateValue(J)V

    invoke-static {}, Lorg/codroid/utility/LocalWriter;->commit()V

    return-void
.end method

.method public static valueBC()V
    .locals 2

    .prologue
    .line 37
    const-wide/16 v0, 0xb

    invoke-static {v0, v1}, Lorg/codroid/utility/LocalWriter;->updateValue(J)V

    invoke-static {}, Lorg/codroid/utility/LocalWriter;->commit()V

    return-void
.end method

.method public static valueCC()V
    .locals 2

    .prologue
    .line 38
    const-wide/16 v0, 0xc

    invoke-static {v0, v1}, Lorg/codroid/utility/LocalWriter;->updateValue(J)V

    invoke-static {}, Lorg/codroid/utility/LocalWriter;->commit()V

    return-void
.end method

.method public static valueDC()V
    .locals 2

    .prologue
    .line 39
    const-wide/16 v0, 0xd

    invoke-static {v0, v1}, Lorg/codroid/utility/LocalWriter;->updateValue(J)V

    invoke-static {}, Lorg/codroid/utility/LocalWriter;->commit()V

    return-void
.end method

.method public static valueEC()V
    .locals 2

    .prologue
    .line 40
    const-wide/16 v0, 0xe

    invoke-static {v0, v1}, Lorg/codroid/utility/LocalWriter;->updateValue(J)V

    invoke-static {}, Lorg/codroid/utility/LocalWriter;->commit()V

    return-void
.end method

.method public static valueFC()V
    .locals 2

    .prologue
    .line 41
    const-wide/16 v0, 0xf

    invoke-static {v0, v1}, Lorg/codroid/utility/LocalWriter;->updateValue(J)V

    invoke-static {}, Lorg/codroid/utility/LocalWriter;->commit()V

    return-void
.end method

.method public static valueI0C()V
    .locals 2

    .prologue
    .line 43
    const-wide/16 v0, 0x0

    invoke-static {v0, v1}, Lorg/codroid/utility/LocalWriter;->commit(J)V

    return-void
.end method

.method public static valueI1C()V
    .locals 2

    .prologue
    .line 44
    const-wide/16 v0, 0x1

    invoke-static {v0, v1}, Lorg/codroid/utility/LocalWriter;->commit(J)V

    return-void
.end method

.method public static valueI2C()V
    .locals 2

    .prologue
    .line 45
    const-wide/16 v0, 0x2

    invoke-static {v0, v1}, Lorg/codroid/utility/LocalWriter;->commit(J)V

    return-void
.end method

.method public static valueI3C()V
    .locals 2

    .prologue
    .line 46
    const-wide/16 v0, 0x3

    invoke-static {v0, v1}, Lorg/codroid/utility/LocalWriter;->commit(J)V

    return-void
.end method

.method public static valueI4C()V
    .locals 2

    .prologue
    .line 47
    const-wide/16 v0, 0x4

    invoke-static {v0, v1}, Lorg/codroid/utility/LocalWriter;->commit(J)V

    return-void
.end method

.method public static valueI5C()V
    .locals 2

    .prologue
    .line 48
    const-wide/16 v0, 0x5

    invoke-static {v0, v1}, Lorg/codroid/utility/LocalWriter;->commit(J)V

    return-void
.end method

.method public static valueI6C()V
    .locals 2

    .prologue
    .line 49
    const-wide/16 v0, 0x6

    invoke-static {v0, v1}, Lorg/codroid/utility/LocalWriter;->commit(J)V

    return-void
.end method

.method public static valueI7C()V
    .locals 2

    .prologue
    .line 50
    const-wide/16 v0, 0x7

    invoke-static {v0, v1}, Lorg/codroid/utility/LocalWriter;->commit(J)V

    return-void
.end method

.method public static valueI8C()V
    .locals 2

    .prologue
    .line 51
    const-wide/16 v0, 0x8

    invoke-static {v0, v1}, Lorg/codroid/utility/LocalWriter;->commit(J)V

    return-void
.end method

.method public static valueI9C()V
    .locals 2

    .prologue
    .line 52
    const-wide/16 v0, 0x9

    invoke-static {v0, v1}, Lorg/codroid/utility/LocalWriter;->commit(J)V

    return-void
.end method

.method public static valueIAC()V
    .locals 2

    .prologue
    .line 53
    const-wide/16 v0, 0xa

    invoke-static {v0, v1}, Lorg/codroid/utility/LocalWriter;->commit(J)V

    return-void
.end method

.method public static valueIBC()V
    .locals 2

    .prologue
    .line 54
    const-wide/16 v0, 0xb

    invoke-static {v0, v1}, Lorg/codroid/utility/LocalWriter;->commit(J)V

    return-void
.end method

.method public static valueICC()V
    .locals 2

    .prologue
    .line 55
    const-wide/16 v0, 0xc

    invoke-static {v0, v1}, Lorg/codroid/utility/LocalWriter;->commit(J)V

    return-void
.end method

.method public static valueIDC()V
    .locals 2

    .prologue
    .line 56
    const-wide/16 v0, 0xd

    invoke-static {v0, v1}, Lorg/codroid/utility/LocalWriter;->commit(J)V

    return-void
.end method

.method public static valueIEC()V
    .locals 2

    .prologue
    .line 57
    const-wide/16 v0, 0xe

    invoke-static {v0, v1}, Lorg/codroid/utility/LocalWriter;->commit(J)V

    return-void
.end method

.method public static valueIFC()V
    .locals 2

    .prologue
    .line 58
    const-wide/16 v0, 0xf

    invoke-static {v0, v1}, Lorg/codroid/utility/LocalWriter;->commit(J)V

    return-void
.end method
