.class public Lorg/codroid/utility/Statistics;
.super Ljava/lang/Thread;
.source "Statistics.java"


# static fields
.field protected static codroidId:J = 0x0L

.field private static key:Ljava/lang/Long; = null

.field protected static mutex:Ljava/util/concurrent/locks/Lock; = null

.field private static one:Ljava/lang/Long; = null

.field protected static packageName:Ljava/lang/String; = null

.field private static final shift:I = 0x4

.field protected static table:[Ljava/util/Hashtable; = null
    .annotation system Ldalvik/annotation/Signature;
        value = {
            "[",
            "Ljava/util/Hashtable",
            "<",
            "Ljava/lang/Long;",
            "Ljava/lang/Long;",
            ">;"
        }
    .end annotation
.end field

.field protected static tableIndex:I = 0x0

.field protected static task:Lorg/codroid/utility/StatisticsTask; = null

.field private static thread:Ljava/lang/Thread; = null

.field protected static timestamp:J = 0x0L

.field private static final updateInterval:I = 0x2710

.field private static value:J


# direct methods
.method static constructor <clinit>()V
    .locals 6

    .prologue
    const-wide/16 v4, 0x0

    const/4 v2, 0x0

    .line 19
    sput-wide v4, Lorg/codroid/utility/Statistics;->value:J

    .line 21
    sput-object v2, Lorg/codroid/utility/Statistics;->key:Ljava/lang/Long;

    .line 22
    sput-object v2, Lorg/codroid/utility/Statistics;->one:Ljava/lang/Long;

    .line 23
    sput-object v2, Lorg/codroid/utility/Statistics;->thread:Ljava/lang/Thread;

    .line 25
    const-wide/32 v0, 0x28091512

    sput-wide v0, Lorg/codroid/utility/Statistics;->codroidId:J

    .line 26
    sput-object v2, Lorg/codroid/utility/Statistics;->packageName:Ljava/lang/String;

    .line 27
    sput-object v2, Lorg/codroid/utility/Statistics;->task:Lorg/codroid/utility/StatisticsTask;

    .line 28
    sput-object v2, Lorg/codroid/utility/Statistics;->mutex:Ljava/util/concurrent/locks/Lock;

    .line 29
    sput-wide v4, Lorg/codroid/utility/Statistics;->timestamp:J

    .line 30
    const/4 v0, 0x0

    sput v0, Lorg/codroid/utility/Statistics;->tableIndex:I

    return-void
.end method

.method public constructor <init>()V
    .locals 0

    .prologue
    .line 16
    invoke-direct {p0}, Ljava/lang/Thread;-><init>()V

    return-void
.end method

.method public static clear()V
    .locals 2

    .prologue
    .line 136
    sget-object v0, Lorg/codroid/utility/Statistics;->table:[Ljava/util/Hashtable;

    sget v1, Lorg/codroid/utility/Statistics;->tableIndex:I

    aget-object v0, v0, v1

    invoke-virtual {v0}, Ljava/util/Hashtable;->clear()V

    .line 137
    return-void
.end method

.method public static commit()V
    .locals 8

    .prologue
    .line 107
    const/4 v0, 0x0

    .line 108
    .local v0, "i":Ljava/lang/Long;
    sget-wide v2, Lorg/codroid/utility/Statistics;->value:J

    invoke-static {v2, v3}, Ljava/lang/Long;->valueOf(J)Ljava/lang/Long;

    move-result-object v1

    sput-object v1, Lorg/codroid/utility/Statistics;->key:Ljava/lang/Long;

    .line 109
    sget-object v1, Lorg/codroid/utility/Statistics;->table:[Ljava/util/Hashtable;

    sget v2, Lorg/codroid/utility/Statistics;->tableIndex:I

    aget-object v1, v1, v2

    sget-object v2, Lorg/codroid/utility/Statistics;->key:Ljava/lang/Long;

    invoke-virtual {v1, v2}, Ljava/util/Hashtable;->get(Ljava/lang/Object;)Ljava/lang/Object;

    move-result-object v0

    .end local v0    # "i":Ljava/lang/Long;
    check-cast v0, Ljava/lang/Long;

    .restart local v0    # "i":Ljava/lang/Long;
    if-eqz v0, :cond_0

    .line 110
    sget-object v1, Lorg/codroid/utility/Statistics;->table:[Ljava/util/Hashtable;

    sget v2, Lorg/codroid/utility/Statistics;->tableIndex:I

    aget-object v1, v1, v2

    sget-object v2, Lorg/codroid/utility/Statistics;->key:Ljava/lang/Long;

    invoke-virtual {v0}, Ljava/lang/Long;->longValue()J

    move-result-wide v4

    const-wide/16 v6, 0x1

    add-long/2addr v4, v6

    invoke-static {v4, v5}, Ljava/lang/Long;->valueOf(J)Ljava/lang/Long;

    move-result-object v3

    invoke-virtual {v1, v2, v3}, Ljava/util/Hashtable;->put(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;

    .line 114
    :goto_0
    sget-object v1, Lorg/codroid/utility/Statistics;->mutex:Ljava/util/concurrent/locks/Lock;

    invoke-interface {v1}, Ljava/util/concurrent/locks/Lock;->unlock()V

    .line 115
    return-void

    .line 112
    :cond_0
    sget-object v1, Lorg/codroid/utility/Statistics;->table:[Ljava/util/Hashtable;

    sget v2, Lorg/codroid/utility/Statistics;->tableIndex:I

    aget-object v1, v1, v2

    sget-object v2, Lorg/codroid/utility/Statistics;->key:Ljava/lang/Long;

    sget-object v3, Lorg/codroid/utility/Statistics;->one:Ljava/lang/Long;

    invoke-virtual {v1, v2, v3}, Ljava/util/Hashtable;->put(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;

    goto :goto_0
.end method

.method public static commit(J)V
    .locals 8
    .param p0, "v"    # J

    .prologue
    .line 118
    const/4 v0, 0x0

    .line 119
    .local v0, "i":Ljava/lang/Long;
    invoke-static {}, Lorg/codroid/utility/Statistics;->realInit()V

    .line 120
    sget-object v1, Lorg/codroid/utility/Statistics;->mutex:Ljava/util/concurrent/locks/Lock;

    invoke-interface {v1}, Ljava/util/concurrent/locks/Lock;->lock()V

    .line 121
    invoke-static {p0, p1}, Ljava/lang/Long;->valueOf(J)Ljava/lang/Long;

    move-result-object v1

    sput-object v1, Lorg/codroid/utility/Statistics;->key:Ljava/lang/Long;

    .line 122
    sget-object v1, Lorg/codroid/utility/Statistics;->table:[Ljava/util/Hashtable;

    sget v2, Lorg/codroid/utility/Statistics;->tableIndex:I

    aget-object v1, v1, v2

    sget-object v2, Lorg/codroid/utility/Statistics;->key:Ljava/lang/Long;

    invoke-virtual {v1, v2}, Ljava/util/Hashtable;->get(Ljava/lang/Object;)Ljava/lang/Object;

    move-result-object v0

    .end local v0    # "i":Ljava/lang/Long;
    check-cast v0, Ljava/lang/Long;

    .restart local v0    # "i":Ljava/lang/Long;
    if-eqz v0, :cond_0

    .line 123
    sget-object v1, Lorg/codroid/utility/Statistics;->table:[Ljava/util/Hashtable;

    sget v2, Lorg/codroid/utility/Statistics;->tableIndex:I

    aget-object v1, v1, v2

    sget-object v2, Lorg/codroid/utility/Statistics;->key:Ljava/lang/Long;

    invoke-virtual {v0}, Ljava/lang/Long;->longValue()J

    move-result-wide v4

    const-wide/16 v6, 0x1

    add-long/2addr v4, v6

    invoke-static {v4, v5}, Ljava/lang/Long;->valueOf(J)Ljava/lang/Long;

    move-result-object v3

    invoke-virtual {v1, v2, v3}, Ljava/util/Hashtable;->put(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;

    .line 127
    :goto_0
    sget-object v1, Lorg/codroid/utility/Statistics;->mutex:Ljava/util/concurrent/locks/Lock;

    invoke-interface {v1}, Ljava/util/concurrent/locks/Lock;->unlock()V

    .line 128
    return-void

    .line 125
    :cond_0
    sget-object v1, Lorg/codroid/utility/Statistics;->table:[Ljava/util/Hashtable;

    sget v2, Lorg/codroid/utility/Statistics;->tableIndex:I

    aget-object v1, v1, v2

    sget-object v2, Lorg/codroid/utility/Statistics;->key:Ljava/lang/Long;

    sget-object v3, Lorg/codroid/utility/Statistics;->one:Ljava/lang/Long;

    invoke-virtual {v1, v2, v3}, Ljava/util/Hashtable;->put(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;

    goto :goto_0
.end method

.method public static dump()V
    .locals 9

    .prologue
    .line 140
    sget-object v4, Lorg/codroid/utility/Statistics;->table:[Ljava/util/Hashtable;

    sget v5, Lorg/codroid/utility/Statistics;->tableIndex:I

    aget-object v4, v4, v5

    invoke-virtual {v4}, Ljava/util/Hashtable;->keySet()Ljava/util/Set;

    move-result-object v3

    .line 141
    .local v3, "keys":Ljava/util/Set;, "Ljava/util/Set<Ljava/lang/Long;>;"
    const-wide/16 v0, 0x0

    .line 142
    .local v0, "count":J
    invoke-interface {v3}, Ljava/util/Set;->iterator()Ljava/util/Iterator;

    move-result-object v5

    :goto_0
    invoke-interface {v5}, Ljava/util/Iterator;->hasNext()Z

    move-result v4

    if-eqz v4, :cond_0

    invoke-interface {v5}, Ljava/util/Iterator;->next()Ljava/lang/Object;

    move-result-object v2

    check-cast v2, Ljava/lang/Long;

    .line 143
    .local v2, "i":Ljava/lang/Long;
    const-string v4, "CoDroid"

    new-instance v6, Ljava/lang/StringBuilder;

    invoke-direct {v6}, Ljava/lang/StringBuilder;-><init>()V

    const-string v7, "Dump: "

    invoke-virtual {v6, v7}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v6

    invoke-virtual {v6, v2}, Ljava/lang/StringBuilder;->append(Ljava/lang/Object;)Ljava/lang/StringBuilder;

    move-result-object v6

    const-string v7, ": "

    invoke-virtual {v6, v7}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v6

    sget-object v7, Lorg/codroid/utility/Statistics;->table:[Ljava/util/Hashtable;

    sget v8, Lorg/codroid/utility/Statistics;->tableIndex:I

    aget-object v7, v7, v8

    invoke-virtual {v7, v2}, Ljava/util/Hashtable;->get(Ljava/lang/Object;)Ljava/lang/Object;

    move-result-object v7

    invoke-virtual {v6, v7}, Ljava/lang/StringBuilder;->append(Ljava/lang/Object;)Ljava/lang/StringBuilder;

    move-result-object v6

    invoke-virtual {v6}, Ljava/lang/StringBuilder;->toString()Ljava/lang/String;

    move-result-object v6

    invoke-static {v4, v6}, Landroid/util/Log;->i(Ljava/lang/String;Ljava/lang/String;)I

    .line 144
    sget-object v4, Lorg/codroid/utility/Statistics;->table:[Ljava/util/Hashtable;

    sget v6, Lorg/codroid/utility/Statistics;->tableIndex:I

    aget-object v4, v4, v6

    invoke-virtual {v4, v2}, Ljava/util/Hashtable;->get(Ljava/lang/Object;)Ljava/lang/Object;

    move-result-object v4

    check-cast v4, Ljava/lang/Long;

    invoke-virtual {v4}, Ljava/lang/Long;->longValue()J

    move-result-wide v6

    add-long/2addr v0, v6

    .line 145
    goto :goto_0

    .line 146
    .end local v2    # "i":Ljava/lang/Long;
    :cond_0
    const-string v4, "CoDroid"

    new-instance v5, Ljava/lang/StringBuilder;

    invoke-direct {v5}, Ljava/lang/StringBuilder;-><init>()V

    const-string v6, "Dump: Count: "

    invoke-virtual {v5, v6}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v5

    invoke-virtual {v5, v0, v1}, Ljava/lang/StringBuilder;->append(J)Ljava/lang/StringBuilder;

    move-result-object v5

    invoke-virtual {v5}, Ljava/lang/StringBuilder;->toString()Ljava/lang/String;

    move-result-object v5

    invoke-static {v4, v5}, Landroid/util/Log;->i(Ljava/lang/String;Ljava/lang/String;)I

    .line 147
    return-void
.end method

.method public static getCodroidId()J
    .locals 2

    .prologue
    .line 57
    sget-wide v0, Lorg/codroid/utility/Statistics;->codroidId:J

    return-wide v0
.end method

.method public static getPackageName()Ljava/lang/String;
    .locals 5

    .prologue
    .line 61
    const/4 v1, 0x0

    .line 62
    .local v1, "s":Ljava/lang/String;
    sget-object v2, Lorg/codroid/utility/Statistics;->packageName:Ljava/lang/String;

    if-eqz v2, :cond_0

    .line 63
    sget-object v1, Lorg/codroid/utility/Statistics;->packageName:Ljava/lang/String;

    .line 70
    :goto_0
    return-object v1

    .line 66
    :cond_0
    :try_start_0
    const-string v2, "android.app.ActivityThread"

    invoke-static {v2}, Ljava/lang/Class;->forName(Ljava/lang/String;)Ljava/lang/Class;

    move-result-object v2

    const-string v3, "currentApplication"

    const/4 v4, 0x0

    new-array v4, v4, [Ljava/lang/Class;

    .line 67
    invoke-virtual {v2, v3, v4}, Ljava/lang/Class;->getMethod(Ljava/lang/String;[Ljava/lang/Class;)Ljava/lang/reflect/Method;

    move-result-object v3

    const/4 v4, 0x0

    const/4 v2, 0x0

    check-cast v2, [Ljava/lang/Object;

    invoke-virtual {v3, v4, v2}, Ljava/lang/reflect/Method;->invoke(Ljava/lang/Object;[Ljava/lang/Object;)Ljava/lang/Object;

    move-result-object v0

    check-cast v0, Landroid/app/Application;

    .line 68
    .local v0, "app":Landroid/app/Application;
    invoke-virtual {v0}, Landroid/app/Application;->getApplicationContext()Landroid/content/Context;

    move-result-object v2

    invoke-virtual {v2}, Landroid/content/Context;->getPackageName()Ljava/lang/String;
    :try_end_0
    .catch Ljava/lang/Exception; {:try_start_0 .. :try_end_0} :catch_0

    move-result-object v1

    goto :goto_0

    .line 69
    .end local v0    # "app":Landroid/app/Application;
    :catch_0
    move-exception v2

    goto :goto_0
.end method

.method public static init()V
    .locals 2

    .prologue
    .line 101
    invoke-static {}, Lorg/codroid/utility/Statistics;->realInit()V

    .line 102
    sget-object v0, Lorg/codroid/utility/Statistics;->mutex:Ljava/util/concurrent/locks/Lock;

    invoke-interface {v0}, Ljava/util/concurrent/locks/Lock;->lock()V

    .line 103
    const-wide/16 v0, 0x0

    sput-wide v0, Lorg/codroid/utility/Statistics;->value:J

    .line 104
    return-void
.end method

.method public static declared-synchronized realInit()V
    .locals 4

    .prologue
    .line 74
    const-class v1, Lorg/codroid/utility/Statistics;

    monitor-enter v1

    :try_start_0
    sget-object v0, Lorg/codroid/utility/Statistics;->mutex:Ljava/util/concurrent/locks/Lock;

    if-nez v0, :cond_1

    .line 75
    new-instance v0, Ljava/util/concurrent/locks/ReentrantLock;

    invoke-direct {v0}, Ljava/util/concurrent/locks/ReentrantLock;-><init>()V

    sput-object v0, Lorg/codroid/utility/Statistics;->mutex:Ljava/util/concurrent/locks/Lock;

    .line 76
    invoke-static {}, Lorg/codroid/utility/Statistics;->getPackageName()Ljava/lang/String;

    move-result-object v0

    sput-object v0, Lorg/codroid/utility/Statistics;->packageName:Ljava/lang/String;

    .line 77
    const-string v0, "CoDroid"

    new-instance v2, Ljava/lang/StringBuilder;

    invoke-direct {v2}, Ljava/lang/StringBuilder;-><init>()V

    const-string v3, "Current application: "

    invoke-virtual {v2, v3}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v2

    sget-object v3, Lorg/codroid/utility/Statistics;->packageName:Ljava/lang/String;

    invoke-virtual {v2, v3}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v2

    invoke-virtual {v2}, Ljava/lang/StringBuilder;->toString()Ljava/lang/String;

    move-result-object v2

    invoke-static {v0, v2}, Landroid/util/Log;->i(Ljava/lang/String;Ljava/lang/String;)I

    .line 78
    const/4 v0, 0x2

    new-array v0, v0, [Ljava/util/Hashtable;

    const/4 v2, 0x0

    new-instance v3, Ljava/util/Hashtable;

    invoke-direct {v3}, Ljava/util/Hashtable;-><init>()V

    aput-object v3, v0, v2

    const/4 v2, 0x1

    new-instance v3, Ljava/util/Hashtable;

    invoke-direct {v3}, Ljava/util/Hashtable;-><init>()V

    aput-object v3, v0, v2

    sput-object v0, Lorg/codroid/utility/Statistics;->table:[Ljava/util/Hashtable;

    .line 79
    new-instance v0, Ljava/lang/Long;

    const-wide/16 v2, 0x0

    invoke-direct {v0, v2, v3}, Ljava/lang/Long;-><init>(J)V

    sput-object v0, Lorg/codroid/utility/Statistics;->key:Ljava/lang/Long;

    .line 80
    new-instance v0, Ljava/lang/Long;

    const-wide/16 v2, 0x1

    invoke-direct {v0, v2, v3}, Ljava/lang/Long;-><init>(J)V

    sput-object v0, Lorg/codroid/utility/Statistics;->one:Ljava/lang/Long;

    .line 81
    new-instance v0, Lorg/codroid/utility/Statistics;

    invoke-direct {v0}, Lorg/codroid/utility/Statistics;-><init>()V

    sput-object v0, Lorg/codroid/utility/Statistics;->thread:Ljava/lang/Thread;

    .line 82
    sget-object v0, Lorg/codroid/utility/Statistics;->thread:Ljava/lang/Thread;

    if-eqz v0, :cond_0

    sget-object v0, Lorg/codroid/utility/Statistics;->mutex:Ljava/util/concurrent/locks/Lock;

    if-eqz v0, :cond_0

    sget-object v0, Lorg/codroid/utility/Statistics;->table:[Ljava/util/Hashtable;

    if-eqz v0, :cond_0

    sget-object v0, Lorg/codroid/utility/Statistics;->table:[Ljava/util/Hashtable;

    const/4 v2, 0x0

    aget-object v0, v0, v2

    if-eqz v0, :cond_0

    sget-object v0, Lorg/codroid/utility/Statistics;->table:[Ljava/util/Hashtable;

    const/4 v2, 0x1

    aget-object v0, v0, v2

    if-eqz v0, :cond_0

    sget-object v0, Lorg/codroid/utility/Statistics;->key:Ljava/lang/Long;

    if-eqz v0, :cond_0

    sget-object v0, Lorg/codroid/utility/Statistics;->one:Ljava/lang/Long;

    if-nez v0, :cond_2

    .line 83
    :cond_0
    const-string v0, "CoDroid"

    const-string v2, "Initialization failed."

    invoke-static {v0, v2}, Landroid/util/Log;->e(Ljava/lang/String;Ljava/lang/String;)I
    :try_end_0
    .catchall {:try_start_0 .. :try_end_0} :catchall_0

    .line 88
    :cond_1
    :goto_0
    monitor-exit v1

    return-void

    .line 85
    :cond_2
    :try_start_1
    sget-object v0, Lorg/codroid/utility/Statistics;->thread:Ljava/lang/Thread;

    invoke-virtual {v0}, Ljava/lang/Thread;->start()V
    :try_end_1
    .catchall {:try_start_1 .. :try_end_1} :catchall_0

    goto :goto_0

    .line 74
    :catchall_0
    move-exception v0

    monitor-exit v1

    throw v0
.end method

.method public static declared-synchronized setStatisticsTask(Lorg/codroid/utility/StatisticsTask;)V
    .locals 4
    .param p0, "t"    # Lorg/codroid/utility/StatisticsTask;

    .prologue
    .line 49
    const-class v1, Lorg/codroid/utility/Statistics;

    monitor-enter v1

    :try_start_0
    sget-object v0, Lorg/codroid/utility/Statistics;->task:Lorg/codroid/utility/StatisticsTask;

    if-nez v0, :cond_0

    .line 50
    invoke-static {}, Ljava/lang/System;->currentTimeMillis()J

    move-result-wide v2

    sput-wide v2, Lorg/codroid/utility/Statistics;->timestamp:J

    .line 51
    sput-object p0, Lorg/codroid/utility/Statistics;->task:Lorg/codroid/utility/StatisticsTask;

    .line 52
    sget-object v0, Lorg/codroid/utility/Statistics;->task:Lorg/codroid/utility/StatisticsTask;

    invoke-virtual {v0}, Lorg/codroid/utility/StatisticsTask;->initialize()V
    :try_end_0
    .catchall {:try_start_0 .. :try_end_0} :catchall_0

    .line 54
    :cond_0
    monitor-exit v1

    return-void

    .line 49
    :catchall_0
    move-exception v0

    monitor-exit v1

    throw v0
.end method

.method public static swap()Ljava/util/Hashtable;
    .locals 3
    .annotation system Ldalvik/annotation/Signature;
        value = {
            "()",
            "Ljava/util/Hashtable",
            "<",
            "Ljava/lang/Long;",
            "Ljava/lang/Long;",
            ">;"
        }
    .end annotation

    .prologue
    .line 91
    const/4 v0, 0x0

    .line 92
    .local v0, "ret":Ljava/util/Hashtable;, "Ljava/util/Hashtable<Ljava/lang/Long;Ljava/lang/Long;>;"
    invoke-static {}, Lorg/codroid/utility/Statistics;->realInit()V

    .line 93
    sget-object v1, Lorg/codroid/utility/Statistics;->mutex:Ljava/util/concurrent/locks/Lock;

    invoke-interface {v1}, Ljava/util/concurrent/locks/Lock;->lock()V

    .line 94
    sget-object v1, Lorg/codroid/utility/Statistics;->table:[Ljava/util/Hashtable;

    sget v2, Lorg/codroid/utility/Statistics;->tableIndex:I

    aget-object v0, v1, v2

    .line 95
    sget v1, Lorg/codroid/utility/Statistics;->tableIndex:I

    rsub-int/lit8 v1, v1, 0x1

    sput v1, Lorg/codroid/utility/Statistics;->tableIndex:I

    .line 96
    sget-object v1, Lorg/codroid/utility/Statistics;->mutex:Ljava/util/concurrent/locks/Lock;

    invoke-interface {v1}, Ljava/util/concurrent/locks/Lock;->unlock()V

    .line 97
    return-object v0
.end method

.method public static updateValue(J)V
    .locals 4
    .param p0, "v"    # J

    .prologue
    .line 131
    sget-wide v0, Lorg/codroid/utility/Statistics;->value:J

    const/4 v2, 0x4

    shl-long/2addr v0, v2

    sput-wide v0, Lorg/codroid/utility/Statistics;->value:J

    .line 132
    sget-wide v0, Lorg/codroid/utility/Statistics;->value:J

    or-long/2addr v0, p0

    sput-wide v0, Lorg/codroid/utility/Statistics;->value:J

    .line 133
    return-void
.end method

.method public static value0()V
    .locals 2

    .prologue
    .line 150
    const-wide/16 v0, 0x0

    invoke-static {v0, v1}, Lorg/codroid/utility/Statistics;->updateValue(J)V

    return-void
.end method

.method public static value1()V
    .locals 2

    .prologue
    .line 151
    const-wide/16 v0, 0x1

    invoke-static {v0, v1}, Lorg/codroid/utility/Statistics;->updateValue(J)V

    return-void
.end method

.method public static value2()V
    .locals 2

    .prologue
    .line 152
    const-wide/16 v0, 0x2

    invoke-static {v0, v1}, Lorg/codroid/utility/Statistics;->updateValue(J)V

    return-void
.end method

.method public static value3()V
    .locals 2

    .prologue
    .line 153
    const-wide/16 v0, 0x3

    invoke-static {v0, v1}, Lorg/codroid/utility/Statistics;->updateValue(J)V

    return-void
.end method

.method public static value4()V
    .locals 2

    .prologue
    .line 154
    const-wide/16 v0, 0x4

    invoke-static {v0, v1}, Lorg/codroid/utility/Statistics;->updateValue(J)V

    return-void
.end method

.method public static value5()V
    .locals 2

    .prologue
    .line 155
    const-wide/16 v0, 0x5

    invoke-static {v0, v1}, Lorg/codroid/utility/Statistics;->updateValue(J)V

    return-void
.end method

.method public static value6()V
    .locals 2

    .prologue
    .line 156
    const-wide/16 v0, 0x6

    invoke-static {v0, v1}, Lorg/codroid/utility/Statistics;->updateValue(J)V

    return-void
.end method

.method public static value7()V
    .locals 2

    .prologue
    .line 157
    const-wide/16 v0, 0x7

    invoke-static {v0, v1}, Lorg/codroid/utility/Statistics;->updateValue(J)V

    return-void
.end method

.method public static value8()V
    .locals 2

    .prologue
    .line 158
    const-wide/16 v0, 0x8

    invoke-static {v0, v1}, Lorg/codroid/utility/Statistics;->updateValue(J)V

    return-void
.end method

.method public static value9()V
    .locals 2

    .prologue
    .line 159
    const-wide/16 v0, 0x9

    invoke-static {v0, v1}, Lorg/codroid/utility/Statistics;->updateValue(J)V

    return-void
.end method

.method public static valueA()V
    .locals 2

    .prologue
    .line 160
    const-wide/16 v0, 0xa

    invoke-static {v0, v1}, Lorg/codroid/utility/Statistics;->updateValue(J)V

    return-void
.end method

.method public static valueB()V
    .locals 2

    .prologue
    .line 161
    const-wide/16 v0, 0xb

    invoke-static {v0, v1}, Lorg/codroid/utility/Statistics;->updateValue(J)V

    return-void
.end method

.method public static valueC()V
    .locals 2

    .prologue
    .line 162
    const-wide/16 v0, 0xc

    invoke-static {v0, v1}, Lorg/codroid/utility/Statistics;->updateValue(J)V

    return-void
.end method

.method public static valueD()V
    .locals 2

    .prologue
    .line 163
    const-wide/16 v0, 0xd

    invoke-static {v0, v1}, Lorg/codroid/utility/Statistics;->updateValue(J)V

    return-void
.end method

.method public static valueE()V
    .locals 2

    .prologue
    .line 164
    const-wide/16 v0, 0xe

    invoke-static {v0, v1}, Lorg/codroid/utility/Statistics;->updateValue(J)V

    return-void
.end method

.method public static valueF()V
    .locals 2

    .prologue
    .line 165
    const-wide/16 v0, 0xf

    invoke-static {v0, v1}, Lorg/codroid/utility/Statistics;->updateValue(J)V

    return-void
.end method

.method public static valueI0()V
    .locals 2

    .prologue
    .line 167
    invoke-static {}, Lorg/codroid/utility/Statistics;->init()V

    const-wide/16 v0, 0x0

    invoke-static {v0, v1}, Lorg/codroid/utility/Statistics;->updateValue(J)V

    return-void
.end method

.method public static valueI1()V
    .locals 2

    .prologue
    .line 168
    invoke-static {}, Lorg/codroid/utility/Statistics;->init()V

    const-wide/16 v0, 0x1

    invoke-static {v0, v1}, Lorg/codroid/utility/Statistics;->updateValue(J)V

    return-void
.end method

.method public static valueI2()V
    .locals 2

    .prologue
    .line 169
    invoke-static {}, Lorg/codroid/utility/Statistics;->init()V

    const-wide/16 v0, 0x2

    invoke-static {v0, v1}, Lorg/codroid/utility/Statistics;->updateValue(J)V

    return-void
.end method

.method public static valueI3()V
    .locals 2

    .prologue
    .line 170
    invoke-static {}, Lorg/codroid/utility/Statistics;->init()V

    const-wide/16 v0, 0x3

    invoke-static {v0, v1}, Lorg/codroid/utility/Statistics;->updateValue(J)V

    return-void
.end method

.method public static valueI4()V
    .locals 2

    .prologue
    .line 171
    invoke-static {}, Lorg/codroid/utility/Statistics;->init()V

    const-wide/16 v0, 0x4

    invoke-static {v0, v1}, Lorg/codroid/utility/Statistics;->updateValue(J)V

    return-void
.end method

.method public static valueI5()V
    .locals 2

    .prologue
    .line 172
    invoke-static {}, Lorg/codroid/utility/Statistics;->init()V

    const-wide/16 v0, 0x5

    invoke-static {v0, v1}, Lorg/codroid/utility/Statistics;->updateValue(J)V

    return-void
.end method

.method public static valueI6()V
    .locals 2

    .prologue
    .line 173
    invoke-static {}, Lorg/codroid/utility/Statistics;->init()V

    const-wide/16 v0, 0x6

    invoke-static {v0, v1}, Lorg/codroid/utility/Statistics;->updateValue(J)V

    return-void
.end method

.method public static valueI7()V
    .locals 2

    .prologue
    .line 174
    invoke-static {}, Lorg/codroid/utility/Statistics;->init()V

    const-wide/16 v0, 0x7

    invoke-static {v0, v1}, Lorg/codroid/utility/Statistics;->updateValue(J)V

    return-void
.end method

.method public static valueI8()V
    .locals 2

    .prologue
    .line 175
    invoke-static {}, Lorg/codroid/utility/Statistics;->init()V

    const-wide/16 v0, 0x8

    invoke-static {v0, v1}, Lorg/codroid/utility/Statistics;->updateValue(J)V

    return-void
.end method

.method public static valueI9()V
    .locals 2

    .prologue
    .line 176
    invoke-static {}, Lorg/codroid/utility/Statistics;->init()V

    const-wide/16 v0, 0x9

    invoke-static {v0, v1}, Lorg/codroid/utility/Statistics;->updateValue(J)V

    return-void
.end method

.method public static valueIA()V
    .locals 2

    .prologue
    .line 177
    invoke-static {}, Lorg/codroid/utility/Statistics;->init()V

    const-wide/16 v0, 0xa

    invoke-static {v0, v1}, Lorg/codroid/utility/Statistics;->updateValue(J)V

    return-void
.end method

.method public static valueIB()V
    .locals 2

    .prologue
    .line 178
    invoke-static {}, Lorg/codroid/utility/Statistics;->init()V

    const-wide/16 v0, 0xb

    invoke-static {v0, v1}, Lorg/codroid/utility/Statistics;->updateValue(J)V

    return-void
.end method

.method public static valueIC()V
    .locals 2

    .prologue
    .line 179
    invoke-static {}, Lorg/codroid/utility/Statistics;->init()V

    const-wide/16 v0, 0xc

    invoke-static {v0, v1}, Lorg/codroid/utility/Statistics;->updateValue(J)V

    return-void
.end method

.method public static valueID()V
    .locals 2

    .prologue
    .line 180
    invoke-static {}, Lorg/codroid/utility/Statistics;->init()V

    const-wide/16 v0, 0xd

    invoke-static {v0, v1}, Lorg/codroid/utility/Statistics;->updateValue(J)V

    return-void
.end method

.method public static valueIE()V
    .locals 2

    .prologue
    .line 181
    invoke-static {}, Lorg/codroid/utility/Statistics;->init()V

    const-wide/16 v0, 0xe

    invoke-static {v0, v1}, Lorg/codroid/utility/Statistics;->updateValue(J)V

    return-void
.end method

.method public static valueIF()V
    .locals 2

    .prologue
    .line 182
    invoke-static {}, Lorg/codroid/utility/Statistics;->init()V

    const-wide/16 v0, 0xf

    invoke-static {v0, v1}, Lorg/codroid/utility/Statistics;->updateValue(J)V

    return-void
.end method


# virtual methods
.method public run()V
    .locals 6

    .prologue
    .line 35
    :goto_0
    sget-object v2, Lorg/codroid/utility/Statistics;->task:Lorg/codroid/utility/StatisticsTask;

    if-eqz v2, :cond_0

    .line 36
    invoke-static {}, Ljava/lang/System;->currentTimeMillis()J

    move-result-wide v0

    .line 37
    .local v0, "currentTimestamp":J
    sget-wide v2, Lorg/codroid/utility/Statistics;->timestamp:J

    sub-long v2, v0, v2

    const-wide/16 v4, 0x2710

    cmp-long v2, v2, v4

    if-lez v2, :cond_0

    .line 38
    sget-object v2, Lorg/codroid/utility/Statistics;->task:Lorg/codroid/utility/StatisticsTask;

    invoke-virtual {v2}, Lorg/codroid/utility/StatisticsTask;->action()V

    .line 39
    sput-wide v0, Lorg/codroid/utility/Statistics;->timestamp:J

    .line 43
    .end local v0    # "currentTimestamp":J
    :cond_0
    const-wide/16 v2, 0x3e8

    :try_start_0
    invoke-static {v2, v3}, Ljava/lang/Thread;->sleep(J)V
    :try_end_0
    .catch Ljava/lang/Exception; {:try_start_0 .. :try_end_0} :catch_0

    goto :goto_0

    .line 44
    :catch_0
    move-exception v2

    goto :goto_0
.end method
