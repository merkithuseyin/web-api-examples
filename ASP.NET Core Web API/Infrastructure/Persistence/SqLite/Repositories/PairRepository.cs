using System.Collections.Generic;
using System.Data.SQLite;
using System.Threading;
using System.Threading.Tasks;
using Domain.Entities;
using Domain.RepositoryInterfaces;
using Infrastructure.Persistence.SqLite.Common;

namespace Infrastructure.Persistence.SqLite.Repositories;

public class PairRepository : SqLiteRepository<Pair, int>, IPairRepository
{
    public PairRepository(string connectionString, string? tableName = null) : base(connectionString, tableName)
    {
    }
    
    public async Task<ICollection<Pair>> GetAllAsync(CancellationToken cancellationToken = default)
    {
        var query = $"SELECT * FROM {TableName}";
        await using (var connection = new SQLiteConnection(ConnectionString))
        {
            connection.Open();
            await using (var cmd = new SQLiteCommand(query, connection))
            {
                await using (var rdr = cmd.ExecuteReader())
                {
                    var pairs = new List<Pair>();
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

    public override async Task<Pair?> GetAsync(int id, CancellationToken cancellationToken = default)
    {
        var query = $"SELECT * FROM {TableName} WHERE {TableName}.Id = @id";
        await using (var connection = new SQLiteConnection(ConnectionString))
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

    public override async Task<bool> SaveAsync(Pair entity, CancellationToken cancellationToken = default)
    {
        await using (var connection = new SQLiteConnection(ConnectionString))
        {
            connection.Open();
            await using (var cmd = new SQLiteCommand(connection))
            {
                cmd.CommandText = $"INSERT INTO {TableName}(name, value) VALUES(@name, @value)";
                cmd.Parameters.AddWithValue("@name", entity.Name);
                cmd.Parameters.AddWithValue("@value", entity.Value);
                await cmd.PrepareAsync(cancellationToken);
                
                return await cmd.ExecuteNonQueryAsync(cancellationToken) > 0;
            }
        }
    }

    public override async Task<bool> UpdateAsync(int? id, Pair replacement, CancellationToken cancellationToken = default)
    {
        await using (var connection = new SQLiteConnection(ConnectionString))
        {
            connection.Open();
            await using (var cmd = new SQLiteCommand(connection))
            {
                cmd.CommandText = $"UPDATE {TableName} SET name = @name, value = @value WHERE id = @id";
                cmd.Parameters.AddWithValue("@id",  id ?? replacement.Id);
                cmd.Parameters.AddWithValue("@name", replacement.Name);
                cmd.Parameters.AddWithValue("@value", replacement.Value);
                await cmd.PrepareAsync(cancellationToken);
                
                return await cmd.ExecuteNonQueryAsync(cancellationToken) > 0;
            }
        }
    }
}