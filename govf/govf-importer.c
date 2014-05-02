/*
 * Copyright (C) 2014 Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Author: Zeeshan Ali (Khattak) <zeeshanak@gnome.org>
 */
#include "govf-importer.h"

static void
govf_importer_async_initable_init (GAsyncInitableIface *iface);

G_DEFINE_TYPE_WITH_CODE (GOVFImporter, govf_importer, G_TYPE_OBJECT,
                         G_IMPLEMENT_INTERFACE (G_TYPE_ASYNC_INITABLE,
                                                govf_importer_async_initable_init))

struct _GOVFImporterPrivate
{
        char *ova_file_path;
};

enum
{
        PROP_0,
        PROP_OVA_FILE_PATH,
        LAST_PROP
};

static GParamSpec *gParamSpecs[LAST_PROP];

static void
govf_importer_finalize (GObject *object)
{
        GOVFImporterPrivate *priv;

        priv = GOVF_IMPORTER (object)->priv;
        g_free (priv->ova_file_path);

        G_OBJECT_CLASS (govf_importer_parent_class)->finalize (object);
}

static void
govf_importer_get_property (GObject    *object,
                            guint       prop_id,
                            GValue     *value,
                            GParamSpec *pspec)
{
        GOVFImporter *importer = GOVF_IMPORTER (object);

        switch (prop_id) {
        case PROP_OVA_FILE_PATH:
                g_value_set_string (value, importer->priv->ova_file_path);
                break;

        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        }
}

static void
govf_importer_set_property (GObject      *object,
                            guint         prop_id,
                            const GValue *value,
                            GParamSpec   *pspec)
{
    GOVFImporter *importer = GOVF_IMPORTER (object);

    switch (prop_id) {
    case PROP_OVA_FILE_PATH:
            importer->priv->ova_file_path = g_value_dup_string (value);
            break;

    default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
govf_importer_class_init (GOVFImporterClass *klass)
{
        GObjectClass *object_class;
        GParamSpec *pspec;

        object_class = G_OBJECT_CLASS (klass);
        object_class->finalize = govf_importer_finalize;
        object_class->get_property = govf_importer_get_property;
        object_class->set_property = govf_importer_set_property;
        g_type_class_add_private (object_class, sizeof (GOVFImporterPrivate));

        /**
         * GOVFImporter:ova_file_path:
         *
         * Path to OVA file to import.
         */
        pspec = g_param_spec_string ("ova-file-path",
                                     "OvaFilePath",
                                     "Ova File Path",
                                     NULL /* default value */,
                                     G_PARAM_READWRITE |
                                     G_PARAM_CONSTRUCT_ONLY |
                                     G_PARAM_STATIC_STRINGS);
        g_object_class_install_property (object_class,
                                         PROP_OVA_FILE_PATH,
                                         pspec);
}

static void
govf_importer_init (GOVFImporter *importer)
{
        importer->priv =
                G_TYPE_INSTANCE_GET_PRIVATE (importer,
                                             GOVF_TYPE_IMPORTER,
                                             GOVFImporterPrivate);
}

static void
govf_importer_init_async (GAsyncInitable     *initable,
                          int                 io_priority,
                          GCancellable       *cancellable,
                          GAsyncReadyCallback callback,
                          gpointer            user_data)
{
        GTask *task;

        task = g_task_new (initable, cancellable, callback, user_data);

        g_task_return_boolean (task, TRUE);
        g_object_unref (task);
}

static gboolean
govf_importer_init_finish (GAsyncInitable *initable,
                           GAsyncResult   *res,
                           GError        **error)
{
        g_return_val_if_fail (g_task_is_valid (res, initable), FALSE);

        return g_task_propagate_boolean (G_TASK (res), error);
}

static void
govf_importer_async_initable_init (GAsyncInitableIface *iface)
{
        iface->init_async = govf_importer_init_async;
        iface->init_finish = govf_importer_init_finish;
}

void
govf_importer_new (const char         *ova_file_path,
                   int                 io_priority,
                   GCancellable       *cancellable,
                   GAsyncReadyCallback callback,
                   gpointer            user_data)
{
        g_async_initable_new_async (GOVF_TYPE_IMPORTER,
                                    io_priority,
                                    cancellable,
                                    callback,
                                    user_data,
                                    "ova-file-path",
                                    ova_file_path,
                                    NULL);
}

GOVFImporter *
govf_importer_new_finish (GAsyncResult *res,
                          GError      **error)
{
    GObject *object, *source_object;

    source_object = g_async_result_get_source_object (res);
    object = g_async_initable_new_finish (G_ASYNC_INITABLE (source_object),
                                          res,
                                          error);

    return GOVF_IMPORTER (object);
}

void
govf_importer_import_domain (GOVFImporter       *importer,
                             GVirConnection     *connection,
                             GVirStoragePool    *pool,
                             const char         *storage_dir_path,
                             int                 io_priority,
                             GCancellable       *cancellable,
                             GAsyncReadyCallback callback,
                             gpointer            user_data)
{
        GTask *task;

        task = g_task_new (importer, cancellable, callback, user_data);

        g_task_return_boolean (task, TRUE);
        g_object_unref (task);
}

GVirDomain *
govf_importer_import_domain_finish (GOVFImporter *importer,
                                    GAsyncResult *res,
                                    GError      **error)
{
        *error = g_error_new_literal (G_IO_ERROR,
                                      G_IO_ERROR_NOT_SUPPORTED,
                                      "Not implemented yet");
        return NULL;
}

GVirConfigDomain *
govf_importer_import_config (GOVFImporter *importer,
                             const char   *storage_dir_path,
                             GError      **error)
{
        *error = g_error_new_literal (G_IO_ERROR,
                                      G_IO_ERROR_NOT_SUPPORTED,
                                      "Not implemented yet");
        return NULL;
}

GList *
govf_importer_list_disks (GOVFImporter *importer)
{
        return NULL;
}

void
govf_importer_extract_disk (GOVFImporter       *importer,
                            const char         *name,
                            const char         *storage_dir_path,
                            int                 io_priority,
                            GCancellable       *cancellable,
                            GAsyncReadyCallback callback,
                            gpointer            user_data)
{
        GTask *task;

        task = g_task_new (importer, cancellable, callback, user_data);

        g_task_return_boolean (task, TRUE);
        g_object_unref (task);
}

GFile *
govf_importer_extract_disk_finish (GOVFImporter *importer,
                                   GAsyncResult *res,
                                   GError      **error)
{
        *error = g_error_new_literal (G_IO_ERROR,
                                      G_IO_ERROR_NOT_SUPPORTED,
                                      "Not implemented yet");
        return NULL;
}
