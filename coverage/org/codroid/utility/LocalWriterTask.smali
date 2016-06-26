.class public Lorg/codroid/utility/LocalWriterTask;
.super Lorg/codroid/utility/StatisticsTask;
.source "LocalWriterTask.java"


# instance fields
.field private fout:Ljava/io/DataOutputStream;

.field private initialized:Z


# direct methods
.method public constructor <init>()V
    .locals 1

    .prologue
    .line 15
    invoke-direct {p0}, Lorg/codroid/utility/StatisticsTask;-><init>()V

    .line 16
    const/4 v0, 0x0

    iput-object v0, p0, Lorg/codroid/utility/LocalWriterTask;->fout:Ljava/io/DataOutputStream;

    .line 17
    const/4 v0, 0x0

    iput-boolean v0, p0, Lorg/codroid/utility/LocalWriterTask;->initialized:Z

    return-void
.end method


# virtual methods
.method public action()V
    .locals 10

    .prologue
    .line 65
    const/4 v3, 0x0

    .line 67
    .local v3, "table":Ljava/util/Hashtable;, "Ljava/util/Hashtable<Ljava/lang/Long;Ljava/lang/Long;>;"
    invoke-static {}, Lorg/codroid/utility/LocalWriter;->swap()Ljava/util/Hashtable;

    move-result-object v3

    .line 69
    invoke-virtual {p0}, Lorg/codroid/utility/LocalWriterTask;->initialize()V

    .line 71
    iget-object v4, p0, Lorg/codroid/utility/LocalWriterTask;->fout:Ljava/io/DataOutputStream;

    if-eqz v4, :cond_1

    if-eqz v3, :cond_1

    .line 72
    invoke-virtual {v3}, Ljava/util/Hashtable;->keySet()Ljava/util/Set;

    move-result-object v2

    .line 75
    .local v2, "keys":Ljava/util/Set;, "Ljava/util/Set<Ljava/lang/Long;>;"
    :try_start_0
    iget-object v4, p0, Lorg/codroid/utility/LocalWriterTask;->fout:Ljava/io/DataOutputStream;

    const-wide/16 v6, -0x1

    invoke-virtual {v4, v6, v7}, Ljava/io/DataOutputStream;->writeLong(J)V

    .line 76
    iget-object v4, p0, Lorg/codroid/utility/LocalWriterTask;->fout:Ljava/io/DataOutputStream;

    invoke-static {}, Ljava/lang/System;->currentTimeMillis()J

    move-result-wide v6

    invoke-virtual {v4, v6, v7}, Ljava/io/DataOutputStream;->writeLong(J)V
    :try_end_0
    .catch Ljava/lang/Exception; {:try_start_0 .. :try_end_0} :catch_1

    .line 81
    :goto_0
    invoke-interface {v2}, Ljava/util/Set;->iterator()Ljava/util/Iterator;

    move-result-object v5

    :goto_1
    invoke-interface {v5}, Ljava/util/Iterator;->hasNext()Z

    move-result v4

    if-eqz v4, :cond_0

    invoke-interface {v5}, Ljava/util/Iterator;->next()Ljava/lang/Object;

    move-result-object v1

    check-cast v1, Ljava/lang/Long;

    .line 83
    .local v1, "i":Ljava/lang/Long;
    :try_start_1
    iget-object v4, p0, Lorg/codroid/utility/LocalWriterTask;->fout:Ljava/io/DataOutputStream;

    invoke-virtual {v1}, Ljava/lang/Long;->longValue()J

    move-result-wide v6

    invoke-virtual {v4, v6, v7}, Ljava/io/DataOutputStream;->writeLong(J)V

    .line 84
    iget-object v6, p0, Lorg/codroid/utility/LocalWriterTask;->fout:Ljava/io/DataOutputStream;

    invoke-virtual {v3, v1}, Ljava/util/Hashtable;->get(Ljava/lang/Object;)Ljava/lang/Object;

    move-result-object v4

    check-cast v4, Ljava/lang/Long;

    invoke-virtual {v4}, Ljava/lang/Long;->longValue()J

    move-result-wide v8

    invoke-virtual {v6, v8, v9}, Ljava/io/DataOutputStream;->writeLong(J)V
    :try_end_1
    .catch Ljava/lang/Exception; {:try_start_1 .. :try_end_1} :catch_0

    goto :goto_1

    .line 85
    :catch_0
    move-exception v0

    .line 86
    .local v0, "e":Ljava/lang/Exception;
    const-string v4, "CoDroid"

    new-instance v6, Ljava/lang/StringBuilder;

    invoke-direct {v6}, Ljava/lang/StringBuilder;-><init>()V

    const-string v7, "Write data failed: "

    invoke-virtual {v6, v7}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v6

    invoke-virtual {v0}, Ljava/lang/Exception;->toString()Ljava/lang/String;

    move-result-object v7

    invoke-virtual {v6, v7}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v6

    invoke-virtual {v6}, Ljava/lang/StringBuilder;->toString()Ljava/lang/String;

    move-result-object v6

    invoke-static {v4, v6}, Landroid/util/Log;->e(Ljava/lang/String;Ljava/lang/String;)I

    goto :goto_1

    .line 77
    .end local v0    # "e":Ljava/lang/Exception;
    .end local v1    # "i":Ljava/lang/Long;
    :catch_1
    move-exception v0

    .line 78
    .restart local v0    # "e":Ljava/lang/Exception;
    const-string v4, "CoDroid"

    new-instance v5, Ljava/lang/StringBuilder;

    invoke-direct {v5}, Ljava/lang/StringBuilder;-><init>()V

    const-string v6, "Write data failed: "

    invoke-virtual {v5, v6}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v5

    invoke-virtual {v0}, Ljava/lang/Exception;->toString()Ljava/lang/String;

    move-result-object v6

    invoke-virtual {v5, v6}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v5

    invoke-virtual {v5}, Ljava/lang/StringBuilder;->toString()Ljava/lang/String;

    move-result-object v5

    invoke-static {v4, v5}, Landroid/util/Log;->e(Ljava/lang/String;Ljava/lang/String;)I

    goto :goto_0

    .line 90
    .end local v0    # "e":Ljava/lang/Exception;
    :cond_0
    invoke-virtual {v3}, Ljava/util/Hashtable;->clear()V

    .line 93
    .end local v2    # "keys":Ljava/util/Set;, "Ljava/util/Set<Ljava/lang/Long;>;"
    :cond_1
    return-void
.end method

.method public initialize()V
    .locals 15

    .prologue
    const/16 v14, 0x8

    .line 20
    iget-boolean v8, p0, Lorg/codroid/utility/LocalWriterTask;->initialized:Z

    if-eqz v8, :cond_1

    .line 61
    :cond_0
    :goto_0
    return-void

    .line 23
    :cond_1
    iget-object v8, p0, Lorg/codroid/utility/LocalWriterTask;->fout:Ljava/io/DataOutputStream;

    if-nez v8, :cond_2

    .line 24
    invoke-static {}, Landroid/os/Environment;->getExternalStorageDirectory()Ljava/io/File;

    move-result-object v5

    .line 25
    .local v5, "sdCard":Ljava/io/File;
    new-instance v2, Ljava/io/File;

    new-instance v8, Ljava/lang/StringBuilder;

    invoke-direct {v8}, Ljava/lang/StringBuilder;-><init>()V

    invoke-virtual {v5}, Ljava/io/File;->getAbsolutePath()Ljava/lang/String;

    move-result-object v9

    invoke-virtual {v8, v9}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v8

    const-string v9, "/codroid."

    invoke-virtual {v8, v9}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v8

    invoke-static {}, Ljava/lang/System;->currentTimeMillis()J

    move-result-wide v10

    const-wide/16 v12, 0x3e8

    div-long/2addr v10, v12

    invoke-virtual {v8, v10, v11}, Ljava/lang/StringBuilder;->append(J)Ljava/lang/StringBuilder;

    move-result-object v8

    const-string v9, ".out"

    invoke-virtual {v8, v9}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v8

    invoke-virtual {v8}, Ljava/lang/StringBuilder;->toString()Ljava/lang/String;

    move-result-object v8

    invoke-direct {v2, v8}, Ljava/io/File;-><init>(Ljava/lang/String;)V

    .line 27
    .local v2, "file":Ljava/io/File;
    :try_start_0
    new-instance v8, Ljava/io/DataOutputStream;

    new-instance v9, Ljava/io/FileOutputStream;

    invoke-direct {v9, v2}, Ljava/io/FileOutputStream;-><init>(Ljava/io/File;)V

    invoke-direct {v8, v9}, Ljava/io/DataOutputStream;-><init>(Ljava/io/OutputStream;)V

    iput-object v8, p0, Lorg/codroid/utility/LocalWriterTask;->fout:Ljava/io/DataOutputStream;
    :try_end_0
    .catch Ljava/lang/Exception; {:try_start_0 .. :try_end_0} :catch_0

    .line 33
    .end local v2    # "file":Ljava/io/File;
    .end local v5    # "sdCard":Ljava/io/File;
    :cond_2
    :goto_1
    iget-object v8, p0, Lorg/codroid/utility/LocalWriterTask;->fout:Ljava/io/DataOutputStream;

    if-eqz v8, :cond_0

    iget-boolean v8, p0, Lorg/codroid/utility/LocalWriterTask;->initialized:Z

    if-nez v8, :cond_0

    .line 36
    :try_start_1
    const-string v7, "\u0000\u0000\u0000\u0000\u0000\u0000\u0000\u0000"

    .line 37
    .local v7, "zeros":Ljava/lang/String;
    invoke-static {}, Lorg/codroid/utility/Statistics;->getPackageName()Ljava/lang/String;

    move-result-object v8

    const-string v9, "UTF-8"

    invoke-virtual {v8, v9}, Ljava/lang/String;->getBytes(Ljava/lang/String;)[B

    move-result-object v0

    .line 38
    .local v0, "b":[B
    const-string v8, "UTF-8"

    invoke-virtual {v7, v8}, Ljava/lang/String;->getBytes(Ljava/lang/String;)[B

    move-result-object v6

    .line 39
    .local v6, "z":[B
    const/4 v4, 0x0

    .local v4, "offset":I
    array-length v3, v0

    .line 40
    .local v3, "length":I
    iget-object v8, p0, Lorg/codroid/utility/LocalWriterTask;->fout:Ljava/io/DataOutputStream;

    const-wide v10, -0x1032546f87a9cbeeL    # -3.5987094278483163E230

    invoke-virtual {v8, v10, v11}, Ljava/io/DataOutputStream;->writeLong(J)V

    .line 41
    iget-object v8, p0, Lorg/codroid/utility/LocalWriterTask;->fout:Ljava/io/DataOutputStream;

    invoke-static {}, Lorg/codroid/utility/Statistics;->getCodroidId()J

    move-result-wide v10

    invoke-virtual {v8, v10, v11}, Ljava/io/DataOutputStream;->writeLong(J)V

    .line 42
    :goto_2
    if-lez v3, :cond_4

    .line 43
    iget-object v8, p0, Lorg/codroid/utility/LocalWriterTask;->fout:Ljava/io/DataOutputStream;

    const-wide v10, -0x1032546f87a9cbeeL    # -3.5987094278483163E230

    invoke-virtual {v8, v10, v11}, Ljava/io/DataOutputStream;->writeLong(J)V

    .line 44
    if-le v3, v14, :cond_3

    .line 45
    iget-object v8, p0, Lorg/codroid/utility/LocalWriterTask;->fout:Ljava/io/DataOutputStream;

    const/16 v9, 0x8

    invoke-virtual {v8, v0, v4, v9}, Ljava/io/DataOutputStream;->write([BII)V
    :try_end_1
    .catch Ljava/lang/Exception; {:try_start_1 .. :try_end_1} :catch_1

    .line 46
    add-int/lit8 v4, v4, 0x8

    .line 47
    add-int/lit8 v3, v3, -0x8

    goto :goto_2

    .line 28
    .end local v0    # "b":[B
    .end local v3    # "length":I
    .end local v4    # "offset":I
    .end local v6    # "z":[B
    .end local v7    # "zeros":Ljava/lang/String;
    .restart local v2    # "file":Ljava/io/File;
    .restart local v5    # "sdCard":Ljava/io/File;
    :catch_0
    move-exception v1

    .line 29
    .local v1, "e":Ljava/lang/Exception;
    const-string v8, "CoDroid"

    new-instance v9, Ljava/lang/StringBuilder;

    invoke-direct {v9}, Ljava/lang/StringBuilder;-><init>()V

    const-string v10, "Unable to open output "

    invoke-virtual {v9, v10}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v9

    invoke-virtual {v2}, Ljava/io/File;->getAbsolutePath()Ljava/lang/String;

    move-result-object v10

    invoke-virtual {v9, v10}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v9

    invoke-virtual {v9}, Ljava/lang/StringBuilder;->toString()Ljava/lang/String;

    move-result-object v9

    invoke-static {v8, v9}, Landroid/util/Log;->e(Ljava/lang/String;Ljava/lang/String;)I

    goto :goto_1

    .line 49
    .end local v1    # "e":Ljava/lang/Exception;
    .end local v2    # "file":Ljava/io/File;
    .end local v5    # "sdCard":Ljava/io/File;
    .restart local v0    # "b":[B
    .restart local v3    # "length":I
    .restart local v4    # "offset":I
    .restart local v6    # "z":[B
    .restart local v7    # "zeros":Ljava/lang/String;
    :cond_3
    :try_start_2
    iget-object v8, p0, Lorg/codroid/utility/LocalWriterTask;->fout:Ljava/io/DataOutputStream;

    invoke-virtual {v8, v0, v4, v3}, Ljava/io/DataOutputStream;->write([BII)V

    .line 50
    iget-object v8, p0, Lorg/codroid/utility/LocalWriterTask;->fout:Ljava/io/DataOutputStream;

    const/4 v9, 0x0

    rsub-int/lit8 v10, v3, 0x8

    invoke-virtual {v8, v6, v9, v10}, Ljava/io/DataOutputStream;->write([BII)V
    :try_end_2
    .catch Ljava/lang/Exception; {:try_start_2 .. :try_end_2} :catch_1

    .line 51
    add-int/2addr v4, v3

    .line 52
    const/4 v3, 0x0

    goto :goto_2

    .line 55
    .end local v0    # "b":[B
    .end local v3    # "length":I
    .end local v4    # "offset":I
    .end local v6    # "z":[B
    .end local v7    # "zeros":Ljava/lang/String;
    :catch_1
    move-exception v1

    .line 56
    .restart local v1    # "e":Ljava/lang/Exception;
    const-string v8, "CoDroid"

    new-instance v9, Ljava/lang/StringBuilder;

    invoke-direct {v9}, Ljava/lang/StringBuilder;-><init>()V

    const-string v10, "Write endian data failed: "

    invoke-virtual {v9, v10}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v9

    invoke-virtual {v1}, Ljava/lang/Exception;->toString()Ljava/lang/String;

    move-result-object v10

    invoke-virtual {v9, v10}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v9

    invoke-virtual {v9}, Ljava/lang/StringBuilder;->toString()Ljava/lang/String;

    move-result-object v9

    invoke-static {v8, v9}, Landroid/util/Log;->e(Ljava/lang/String;Ljava/lang/String;)I

    .line 58
    .end local v1    # "e":Ljava/lang/Exception;
    :cond_4
    const/4 v8, 0x1

    iput-boolean v8, p0, Lorg/codroid/utility/LocalWriterTask;->initialized:Z

    .line 59
    const-string v8, "CoDroid"

    const-string v9, "LocalWriter: initialized"

    invoke-static {v8, v9}, Landroid/util/Log;->i(Ljava/lang/String;Ljava/lang/String;)I

    goto/16 :goto_0
.end method
