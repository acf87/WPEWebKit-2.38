# -*- coding: utf-8 -*-
# Generated by Django 1.11.16 on 2022-07-25 23:18
from __future__ import unicode_literals

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('ews', '0007_rename_patch_model_to_change'),
    ]

    operations = [
        migrations.AddField(
            model_name='change',
            name='comment_id',
            field=models.IntegerField(default=-1),
        ),
    ]