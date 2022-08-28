using System;
using System.Data.SQLite;
using System.Threading;
using System.Threading.Tasks;
using Domain.Common;

namespace Infrastructure.Persistence.SqLite.Common;

public class SqLiteRepository<T, TKey> : IRepository<T, TKey> where T : BaseEntity<TKey> where TKey : struct
{
    protected string ConnectionString {get; }
    protected string TableName { get; }

    /// <param name="connectionString"></param>
    /// <param name="tableName">Use this parameter to clarify table name. If not used, table name will be taken from T type</param>
    protected SqLiteRepository(string connectionString, string? tableName = null)
    {
        ConnectionString = connectionString;
        TableName = (!string.IsNullOrWhiteSpace(tableName) ? tableName : typeof(T).Name) ?? throw new InvalidOperationException();
    }

    public virtual Task<T?> GetAsync(TKey id, CancellationToken cancellationToken = default)
    {
        throw new NotImplementedException();
    }

    public virtual Task<bool> SaveAsync(T entity, CancellationToken cancellationToken = default)
    {
        throw new NotImplementedException();
    }

    public virtual Task<bool> UpdateAsync(TKey? id, T replacement, CancellationToken cancellationToken = default)
    {
        throw new NotImplementedException();
    }

    public virtual async Task<bool> DeleteAsync(TKey id, CancellationToken cancellationToken = default)
    {
        await using (var connection = new SQLiteConnection(ConnectionString))
        {
            connection.Open();
            await using (var cmd = new SQLiteCommand(connection))
            {
                cmd.CommandText = $"DELETE FROM {TableName} WHERE id = @id";
                cmd.Parameters.AddWithValue("@id", id);
                await cmd.PrepareAsync(cancellationToken);
                
                return await cmd.ExecuteNonQueryAsync(cancellationToken) > 0;
            }
        }
    }
}