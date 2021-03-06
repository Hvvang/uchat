#include "server.h"

gint get_user_id_run(sqlite3_stmt *stmt, t_client *client) {
    gint rc = 0;

    if ((rc = sqlite3_step(stmt)) == SQLITE_DONE)
        client->uid = -1;
    else if (rc == SQLITE_ROW)
        client->uid = sqlite3_column_int(stmt, 0);
    else 
        g_warning("get_user_id_run step rc:%d\n", rc);
    if ((rc = sqlite3_finalize(stmt)) != SQLITE_OK)
        g_warning("get_user_id_run finalize rc:%d\n", rc);
    return rc;
}

gint get_user_id_prepare(cJSON *root, sqlite3_stmt **stmt) {
    sqlite3 *db = *(mx_get_db());
    gchar *quary = "SELECT user_id, login, passwd_hash FROM users_credential \
                    WHERE login = ? AND passwd_hash = ?;";
    gchar *login = cJSON_GetObjectItem(root, "login")->valuestring;
    gchar *passwd = cJSON_GetObjectItem(root, "password")->valuestring;
    gint rc = 0;

    if ((rc = sqlite3_prepare_v2(db, quary, -1, stmt, NULL)) != SQLITE_OK)
        g_warning("get_user_id_prepare prepare: %d\n", rc);
    if ((rc = sqlite3_bind_text(*stmt, 1, login, -1, NULL)) != SQLITE_OK)
        g_warning("get_user_id_prepare bind: login:%s %d\n", login, rc);
    if ((rc = sqlite3_bind_text(*stmt, 2, passwd, -1, NULL)) != SQLITE_OK)
        g_warning("get_user_id_prepare bind: passwd:%s %d\n", passwd, rc);
    return rc;
}

//TODO: write function which will send erro response
void mx_sign_in(cJSON *root, t_client *client) {
    GHashTable **online_users = mx_get_online_users();
    sqlite3_stmt *stmt = NULL;

    if (get_user_id_prepare(root, &stmt) != SQLITE_OK)
        g_warning("get_user_id_prepare failed\n");
        // TODO: send error?;
    if (get_user_id_run(stmt, client) != SQLITE_OK)
        g_warning("get_user_id_run failed\n");
        // TODO: send error?;
    else {
        if (client->uid == -1)
            mx_send_data(client->data_out, "sign_in failed\n");
        else if (client->uid > 0) {
            g_print("%lld\n", client->uid);
            mx_send_data(client->data_out, "sign_in successfully\n");
            g_hash_table_insert(*online_users, &(client->uid), client);
        }
    }
}
