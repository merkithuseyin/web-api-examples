using System.Data.SQLite;

namespace Infrastructure.Persistence.SqLite.Common;

public static class SqLiteHelper
{
    private static string ConnectionString => "Data Source=:memory:?cache=shared";

    public static void PrepareDatabase()
    {
        using (var connection = new SQLiteConnection(ConnectionString))
        {
            connection.Open();
            using (var cmd = new SQLiteCommand(connection))
            {
                cmd.CommandText = "DROP TABLE IF EXISTS pair";
                cmd.ExecuteNonQuery();

                cmd.CommandText = "CREATE TABLE pair(id INTEGER PRIMARY KEY, name TEXT, value TEXT)";
                cmd.ExecuteNonQuery();
            }
        }
    }
}