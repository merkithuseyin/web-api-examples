using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;
using Core.Models;
using Data_Access;

namespace Business_Logic;

public class PairBLL: IPairBLL
{
    private IPairDAL _pairDal;
    
    public PairBLL()
    {
        _pairDal = new PairDAL();
    }
    
    public async Task<ICollection<IPair>> GetAllAsync(CancellationToken cancellationToken = default)
    {
        return await _pairDal.GetAllAsync(cancellationToken);
    }

    public async Task<IPair?> GetAsync(int id, CancellationToken cancellationToken = default)
    {
        return await _pairDal.GetAsync(id, cancellationToken);
    }

    public async Task<bool> SaveAsync(IPair entity, CancellationToken cancellationToken = default)
    {
        return await _pairDal.SaveAsync(entity, cancellationToken);
    }

    public async Task<bool> UpdateAsync(int? id, IPair entity, CancellationToken cancellationToken = default)
    {
        return await _pairDal.UpdateAsync(id ?? entity.Id, entity, cancellationToken);
    }

    public async Task<bool> DeleteAsync(int id, CancellationToken cancellationToken = default)
    {
        return await _pairDal.DeleteAsync(id, cancellationToken);
    }
}