using System.Collections.Generic;
using System.Data.SQLite;
using System.Threading;
using System.Threading.Tasks;
using Core.Models;
using Data_Access.Models;

namespace Data_Access;

public class PairDAL: IPairDAL
{
    private readonly string _connectionString = SqLiteHelper.ConnectionString;

    public async Task<ICollection<IPair>> GetAllAsync(CancellationToken cancellationToken = default)
    {
        var query = "SELECT * FROM pair";
        await using (var connection = new SQLiteConnection(_connectionString))
        {
            connection.Open();
            await using (var cmd = new SQLiteCommand(query, connection))
            {
                await using (var rdr = cmd.ExecuteReader())
                {
                    var pairs = new List<IPair>();
                    while (rdr.Read())
                    {
                        var idColumn = rdr.GetInt32(0);
                        var nameColumn = rdr.GetString(1);
                        var valueColumn = rdr.GetString(2);

                        var pair = new Pair {Id = idColumn, Name = nameColumn, Value = valueColumn};
                        pairs.Add(pair);
                    }
                    return pairs;
                }
            }
        }
    }

    public async Task<IPair?> GetAsync(int id, CancellationToken cancellationToken = default)
    {
        var query = "SELECT * FROM pair WHERE pair.Id = @id";
        await using (var connection = new SQLiteConnection(_connectionString))
        {
            connection.Open();
            await using (var cmd = new SQLiteCommand(query, connection))
            {
                cmd.Parameters.AddWithValue("@id", id);
                await cmd.PrepareAsync(cancellationToken);

                await using (var rdr = cmd.ExecuteReader())
                {
                    while (rdr.Read())
                    {
                        var idColumn = rdr.GetInt32(0);
                        var nameColumn = rdr.GetString(1);
                        var valueColumn = rdr.GetString(2);

                        var pair = new Pair {Id = idColumn, Name = nameColumn, Value = valueColumn};
                        return pair;
                    }
                }
            }
        }
        return default;
    }
    
    public async Task<bool> SaveAsync(IPair entity, CancellationToken cancellationToken = default)
    {
        await using (var connection = new SQLiteConnection(_connectionString))
        {
            connection.Open();
            await using (var cmd = new SQLiteCommand(connection))
            {
                cmd.CommandText = "INSERT INTO pair(name, value) VALUES(@name, @value)";
                cmd.Parameters.AddWithValue("@name", entity.Name);
                cmd.Parameters.AddWithValue("@value", entity.Value);
                await cmd.PrepareAsync(cancellationToken);
                
                return await cmd.ExecuteNonQueryAsync(cancellationToken) > 0;
            }
        }
    }
    
    public async Task<bool> UpdateAsync(int? id, IPair entity, CancellationToken cancellationToken = default)
    {
        await using (var connection = new SQLiteConnection(_connectionString))
        {
            connection.Open();
            await using (var cmd = new SQLiteCommand(connection))
            {
                cmd.CommandText = "UPDATE pair SET name = @name, value = @value WHERE id = @id";
                cmd.Parameters.AddWithValue("@id", id ?? entity.Id);
                cmd.Parameters.AddWithValue("@name", entity.Name);
                cmd.Parameters.AddWithValue("@value", entity.Value);
                await cmd.PrepareAsync(cancellationToken);
                
                return await cmd.ExecuteNonQueryAsync(cancellationToken) > 0;
            }
        }
    }
    
    public async Task<bool> DeleteAsync(int id, CancellationToken cancellationToken = default)
    {
        await using (var connection = new SQLiteConnection(_connectionString))
        {
            connection.Open();
            await using (var cmd = new SQLiteCommand(connection))
            {
                cmd.CommandText = "DELETE FROM pair WHERE id = @id";
                cmd.Parameters.AddWithValue("@id", id);
                await cmd.PrepareAsync(cancellationToken);
                
                return await cmd.ExecuteNonQueryAsync(cancellationToken) > 0;
            }
        }
    }
}