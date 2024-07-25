#include <libpq-fe.h>
#include <stdio.h>
#include <stdlib.h>

void close_connection(PGconn *conn, int code) {
  PQfinish(conn);
  exit(code);
}

void result_error_handler(PGconn *conn, PGresult *res) {
  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    fprintf(stderr, "%s\n", PQresultErrorMessage(res));
    PQclear(res);
    close_connection(conn, 1);
  } else if (PQntuples(res) == PGRES_EMPTY_QUERY) {
    printf(stderr, "No data retrieved\n");
    PQclear(res);
    close_connection(conn, 1);
  }
}
void command_error_handler(PGconn *conn, PGresult *res) {
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    fprintf(stderr, "%s\n", PQresultErrorMessage(res));
    PQclear(res);
    close_connection(conn, 1);
  }
}
int main(int argc, char *argv[]) {
  printf("libpq tutorial\n");

  // Connect to the database
  // conninfo is a string of keywords and values separated by spaces.
  char *conninfo = "dbname=rust_axum_sqlx user=user "
                   "password=password123 host=127.0.0.1 port=5432";

  // Create a connection
  PGconn *conn = PQconnectdb(conninfo);

  // Check if the connection is successful
  if (PQstatus(conn) != CONNECTION_OK) {
    // If not successful, print the error message and finish the connection
    fprintf(stderr, "Error while connecting to the database server: %s\n",
            PQerrorMessage(conn));

    // Finish the connection
    PQfinish(conn);

    // Exit the program
    exit(1);
  }

  // We have successfully established a connection to the database server
  printf("Connection Established\n");
  printf("Port: %s\n", PQport(conn));
  printf("Host: %s\n", PQhost(conn));
  printf("DBName: %s\n", PQdb(conn));

  // Begin a transaction block
  PGresult *res = PQexec(conn, "BEGIN");
  command_error_handler(conn, res);

  // Query string literal
  char *query = "SELECT * FROM users";

  res = PQexec(conn, query);
  result_error_handler(conn, res);
  PQclear(res);

  // Update user balance and subtract the withdraw amount
  char *udt = "UPDATE account SET balance = balance - $2 WHERE user_id=$1";
  const char *paramValues[2] = {"23", "100"};
  res = PQexecParams(conn, udt, 2, NULL, paramValues, NULL, NULL, 0);
  command_error_handler(conn, res);
  PQclear(res);

  // Commit the transaction
  res = PQexec(conn, "COMMIT");
  command_error_handler(conn, res);
  PQclear(res);

  // retrieve the data
  res = PQexec(conn, query);
  result_error_handler(conn, res);
  printf("Following user updated:\n");
  printf("%s\t%s\t%s\n", PQfname(res, 1), PQfname(res, 2), PQfname(res, 3));
  printf("%s\t%s\t%s\n", PQgetvalue(res, 0, 0), PQgetvalue(res, 0, 1),
         PQgetvalue(res, 0, 2));
  PQclear(res);
  // Close the connection and free the memory
  close_connection(conn, 1);

  return 0;
}
