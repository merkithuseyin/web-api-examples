using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;
using Domain.Common;
using Domain.Entities;

namespace Domain.RepositoryInterfaces;

public interface IPairRepository: IRepository<Pair, int>
{
    
    /// <summary>
    /// Gets all pairs
    /// </summary>
    /// <param name="cancellationToken"></param>
    /// <returns></returns>
    public Task<ICollection<Pair>> GetAllAsync(CancellationToken cancellationToken = default);
}