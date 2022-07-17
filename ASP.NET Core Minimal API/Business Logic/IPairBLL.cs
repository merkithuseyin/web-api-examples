using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;
using Core.Models;

namespace Business_Logic;

public interface IPairBLL
{
    public Task<ICollection<IPair>> GetAllAsync(CancellationToken cancellationToken = default);
    public Task<IPair?> GetAsync(int id, CancellationToken cancellationToken = default);
    public Task<bool> SaveAsync(IPair entity, CancellationToken cancellationToken = default);
    public Task<bool> UpdateAsync(int? id, IPair entity, CancellationToken cancellationToken = default);
    public Task<bool> DeleteAsync(int id, CancellationToken cancellationToken = default);
}