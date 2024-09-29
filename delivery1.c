#include <mysql.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    MYSQL *conn;
    int err;
    MYSQL_RES *result;
    MYSQL_ROW row;

    // Inicializar la conexión al servidor MySQL
    conn = mysql_init(NULL);
    if (conn == NULL) {
        printf("Error creando la conexión: %u %s\n",
               mysql_errno(conn), mysql_error(conn));
        exit(1);
    }
    
    // Establecer la conexión
    conn = mysql_real_connect(conn, "localhost", "root", "mysql", "Competencia", 0, NULL, 0);
    
    if (conn == NULL) {
        printf("Error inicializando la conexión: %u %s\n",
               mysql_errno(conn), mysql_error(conn));
        exit(1);
    }
    
    // Solicitar el nombre del jugador
    char player_name[20];
    printf("Dame el nombre de un jugador\n");
    scanf("%19s", player_name);  // Limitar la entrada para evitar desbordamiento de buffer
    
    char query[256];
    
 // Construir la consulta SQL para obtener las mesas donde juega el jugador
    strcpy(query, "SELECT Encuentro.IDEncuentro, Encuentro.FechaTermino FROM Usuario, Encuentro, Participacion WHERE Usuario.NombreCuenta = '");
    strcat(query, player_name);
    strcat(query, "' AND Usuario.IDUsuario = Participacion.IDUsuario AND Participacion.IDEncuentro = Encuentro.IDEncuentro");
     err = mysql_query(conn, query);
    if (err != 0) {
        printf("Error al consultar la base de datos: %u %s\n",
               mysql_errno(conn), mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }
 result = mysql_store_result(conn);
    if (result == NULL) {
        printf("Error almacenando el resultado: %u %s\n", mysql_errno(conn), mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }
 int num_rows = mysql_num_rows(result);
    if (num_rows == 0) {
        printf("El jugador '%s' no está participando en ninguna mesa o no existe.\n", player_name);
    } else {
        printf("El jugador '%s' está participando en las siguientes mesas:\n", player_name);
        while ((row = mysql_fetch_row(result)) != NULL) {
            printf("Mesa ID: %s, Fecha de Encuentro: %s\n", row[0], row[1]);
        }
    }
 mysql_free_result(result);
    mysql_close(conn);
    exit(0);
}
