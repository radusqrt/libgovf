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

#ifndef GOVF_IMPORTER_H
#define GOVF_IMPORTER_H

#include <libvirt-gobject/libvirt-gobject.h>

G_BEGIN_DECLS

GType
govf_importer_get_type (void) G_GNUC_CONST;

#define GOVF_TYPE_IMPORTER (govf_importer_get_type ())
#define GOVF_IMPORTER(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), GOVF_TYPE_IMPORTER, GOVFImporter))
#define GOVF_IMPORTER_CLASS(obj) (G_TYPE_CHECK_CLASS_CAST ((obj), GOVF_TYPE_IMPORTER, GOVFImporterClass))
#define GOVF_IS_IMPORTER(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GOVF_TYPE_IMPORTER))
#define GOVF_IS_IMPORTER_CLASS(obj) (G_TYPE_CHECK_CLASS_TYPE ((obj), GOVF_TYPE_IMPORTER))
#define GOVF_IMPORTER_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), GOVF_TYPE_IMPORTER, GOVFImporterClass))

typedef struct _GOVFImporterPrivate GOVFImporterPrivate;
typedef struct _GOVFImporter GOVFImporter;
typedef struct _GOVFImporterClass GOVFImporterClass;

struct _GOVFImporter {
        GObject parent;

        GOVFImporterPrivate *priv;
};

struct _GOVFImporterClass {
        GObjectClass parent_class;

        /* future padding */
        void (* _govf_reserved1) (void);
        void (* _govf_reserved2) (void);
        void (* _govf_reserved3) (void);
        void (* _govf_reserved4) (void);
};

void               govf_importer_new                  (const char         *ova_file_path,
                                                       int                 io_priority,
                                                       GCancellable       *cancellable,
                                                       GAsyncReadyCallback callback,
                                                       gpointer            user_data);
GOVFImporter *     govf_importer_new_finish           (GAsyncResult       *res,
                                                       GError            **error);
void               govf_importer_import_domain        (GOVFImporter       *importer,
                                                       GVirConnection     *connection,
                                                       GVirStoragePool    *pool,
                                                       const char         *storage_dir_path,
                                                       int                 io_priority,
                                                       GCancellable       *cancellable,
                                                       GAsyncReadyCallback callback,
                                                       gpointer            user_data);
GVirDomain *       govf_importer_import_domain_finish (GOVFImporter       *importer,
                                                       GAsyncResult       *res,
                                                       GError            **error);
GVirConfigDomain * govf_importer_import_config        (GOVFImporter       *importer,
                                                       const char         *storage_dir_path,
                                                       GError            **error);
GList *            govf_importer_list_disks           (GOVFImporter       *importer);
void               govf_importer_extract_disk         (GOVFImporter       *importer,
                                                       const char         *name,
                                                       const char         *storage_dir_path,
                                                       int                 io_priority,
                                                       GCancellable       *cancellable,
                                                       GAsyncReadyCallback callback,
                                                       gpointer            user_data);
GFile *            govf_importer_extract_disk_finish  (GOVFImporter       *importer,
                                                       GAsyncResult       *res,
                                                       GError            **error);

G_END_DECLS


#endif /* GOVF_IMPORTER_H */
