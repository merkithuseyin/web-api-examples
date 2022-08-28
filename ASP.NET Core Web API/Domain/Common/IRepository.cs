using System.Threading;
using System.Threading.Tasks;

namespace Domain.Common;

public interface IRepository<T, TKey> where TKey : struct
{
    /// <summary>
    /// Gets an entity
    /// </summary>
    /// <param name="id"></param>
    /// <param name="cancellationToken"></param>
    /// <returns></returns>
    public Task<T?> GetAsync(TKey id, CancellationToken cancellationToken = default);

    /// <summary>
    /// Adds an entity.
    /// </summary>
    /// <param name="entity"></param>
    /// <param name="cancellationToken"></param>
    Task<bool> SaveAsync(T entity, CancellationToken cancellationToken = default);

    /// <summary>
    /// Updates an entity.
    /// </summary>
    /// <param name="id">Id of entity that will be updated. If not given, id of "replacement" will be taken</param>
    /// <param name="replacement"></param>
    /// <param name="cancellationToken"></param>
    Task<bool> UpdateAsync(TKey? id, T replacement, CancellationToken cancellationToken = default);

    /// <summary>
    /// Deletes an entity.
    /// </summary>
    /// <param name="id"></param>
    /// <param name="cancellationToken"></param>
    Task<bool> DeleteAsync(TKey id, CancellationToken cancellationToken = default);
}