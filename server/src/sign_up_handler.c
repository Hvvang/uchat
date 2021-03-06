#include "server.h"

gint mx_sign_up_quary(cJSON *root, sqlite3 *db) {
    gchar *quary = "INSERT INTO users_credential(login, passwd_hash) \
                    VALUES(?, ?);";
    sqlite3_stmt *stmt = NULL;
    gchar *login = cJSON_GetObjectItem(root, "login")->valuestring;
    gchar *passwd = cJSON_GetObjectItem(root, "password")->valuestring;
    gint rc = 0;

    if ((rc = sqlite3_prepare_v2(db, quary, -1, &stmt, 0)) != SQLITE_OK)
        g_warning("sign_up_quary prepare: %s\n", sqlite3_errmsg(db));
    if ((sqlite3_bind_text(stmt, 1, login, -1, NULL)) != SQLITE_OK)
        g_warning("sign_up_quary bind:\nlogin:%s\n%s\n",
                   login, sqlite3_errmsg(db));
    if ((sqlite3_bind_text(stmt, 2, passwd, -1, NULL)) != SQLITE_OK)
        g_warning("sign_up_quary bind: login:%s\n%s\n",
                   login, sqlite3_errmsg(db));
    if ((rc = sqlite3_step(stmt)) != SQLITE_DONE)
        g_warning("sign_up_quary step: %s\n", sqlite3_errmsg(db));
    if ((rc = sqlite3_finalize(stmt)) != SQLITE_OK)
        g_warning("sign_up_quary finalize: %s\n", sqlite3_errmsg(db));
    return rc;
}

gboolean mx_check_if_user_excist(cJSON *root, sqlite3 *db) {
    gchar *quary = "SELECT login FROM users_credential WHERE login = ?;";
    sqlite3_stmt *stmt = NULL;
    gchar *login = cJSON_GetObjectItem(root, "login")->valuestring;
    gboolean result = 0;
    gint rc = 0;

    if ((rc = sqlite3_prepare_v2(db, quary, -1, &stmt, NULL)) != SQLITE_OK)
        g_warning("check_if_excist prepare: %s\n", sqlite3_errstr(rc));
    if ((sqlite3_bind_text(stmt, 1, login, -1, NULL)) != SQLITE_OK)
        g_warning("check_if_excist bind: login:%s %s\n",
                   login, sqlite3_errstr(rc));
    if ((rc = sqlite3_step(stmt)) == SQLITE_DONE)
        result = false;
    else if (rc == SQLITE_ROW)
        result = true;
    else
        g_warning("check_if_excist step rc:%d\n", rc);
    if ((rc = sqlite3_finalize(stmt)) != SQLITE_OK)
        g_warning("check_if_excist finalize rc:%d\n", rc);
    return result;
}

void mx_sign_up(cJSON *root, t_client *client) {
    sqlite3 *db = *(mx_get_db());

    if (mx_check_if_user_excist(root, db) == false) {
        mx_sign_up_quary(root, db);
        g_print("User not exsist\n");
    }
    else {
        g_print("User exsist\n");
    }

    (void)client; // Need to use to send response
}

