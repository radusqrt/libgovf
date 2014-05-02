/*
 * vim: ai:ts=8:sw=8:st=8
 */
/*
 * Copyright (C) 2014 Red Hat, Inc.
 *
 * govf-import: Given a path to an OVA file and directory, govf-import will
 *              create a libvirt domain and extract disks into given directory.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * Authors:
 *   Zeeshan Ali <zeenix@redhat.com>
 */

#include <config.h>

#include <govf/govf.h>
#include <string.h>
#include <locale.h>
#include <glib/gi18n.h>

/* Commandline arguments */
char *libvirt_url = "qemu+unix:///session";
char *ova_file = NULL;
char *storage_pool = NULL;

static GOptionEntry entries[] =
{
        { "connect", 'c', 0,
          G_OPTION_ARG_STRING, &libvirt_url,
          N_("Libvirt connection URL. Default: 'qemu+unix:///session'"),
          N_("URL") },
        { NULL }
};

int ret = 0;
GMainLoop *main_loop = NULL;
GOVFImporter *importer = NULL;
GVirConnection *connection = NULL;

void
on_import_domain_ready (GObject      *source_object,
                        GAsyncResult *res,
                        gpointer      user_data)
{
        GVirDomain *domain;
        GError *error = NULL;

        domain = govf_importer_import_domain_finish (importer,
                                                     res,
                                                     &error);
        if (domain == NULL) {
                g_printerr ("Failed to import domain: %s\n", error->message);
                g_main_loop_quit (main_loop);

                return;
        }

        g_print ("%s successfully imported",
                 gvir_domain_get_name (domain));
}

void
on_importer_ready (GObject      *source_object,
                   GAsyncResult *res,
                   gpointer      user_data)
{
        GVirStoragePool *pool;
        GError *error = NULL;

        importer = govf_importer_new_finish (res, &error);
        if (importer == NULL)
                goto error_out;

        connection = gvir_connection_new (libvirt_url);
        if (!gvir_connection_open (connection, NULL, &error))
                goto error_out;
        if (!gvir_connection_fetch_storage_pools (connection, NULL, &error))
                goto error_out;
        pool = gvir_connection_find_storage_pool_by_name (connection,
                                                          storage_pool);
        if (pool == NULL)
                g_print ("No storage pool by name '%s', "  \
                         "assuming its a path to a directory\n",
                         storage_pool);

        govf_importer_import_domain (importer,
                                     connection,
                                     pool,
                                     storage_pool,
                                     G_PRIORITY_HIGH,
                                     NULL,
                                     on_import_domain_ready,
                                     NULL);

        return;

error_out:
        g_printerr ("%s\n", error->message);
        g_main_loop_quit (main_loop);
}

gint
main (gint argc, gchar **argv)
{
        GOptionContext *context;
        GError *error = NULL;

        setlocale (LC_ALL, "");
        textdomain (PACKAGE);
        bindtextdomain (PACKAGE, LOCALEDIR);
        bind_textdomain_codeset (PACKAGE, "UTF-8");

        context = g_option_context_new (_("OVA_FILE STORAGE_POOL_NAME|STORAGE_PATH " \
                                         "- Import OVA file as a libvirt domain"));
        g_option_context_add_main_entries (context, entries, PACKAGE);
        if (!g_option_context_parse (context, &argc, &argv, &error)) {
                g_printerr (_("Error while parsing commandline options: %s\n"),
                           error->message);
                g_printerr ("%s\n", g_option_context_get_help (context, FALSE, NULL));

                ret = -1;
                goto EXIT;
        }

        if (argc < 3) {
                g_printerr ("%s\n", g_option_context_get_help (context, FALSE, NULL));

                ret = -2;
                goto EXIT;
        }

        ova_file = argv[1];
        storage_pool = argv[2];

        govf_importer_new (ova_file,
                           G_PRIORITY_HIGH,
                           NULL,
                           on_importer_ready,
                           NULL);

        main_loop = g_main_loop_new (NULL, FALSE);
        g_main_loop_run (main_loop);

EXIT:
        g_clear_object (&importer);
        g_clear_object (&connection);
        g_clear_pointer (&main_loop, g_main_loop_unref);
        g_clear_error (&error);
        g_option_context_free (context);

        return ret;
}

/*
 * Local variables:
 *  indent-tabs-mode: nil
 *  c-indent-level: 8
 *  c-basic-offset: 8
 * End:
 */
